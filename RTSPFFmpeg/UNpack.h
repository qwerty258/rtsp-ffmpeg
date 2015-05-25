#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include "Decode.h"

typedef struct
{
    unsigned char version;              //!< Version, 2 bits, MUST be 0x2  
    unsigned char padding;              //!< Padding bit, Padding MUST NOT be used  
    unsigned char extension;            //!< Extension, MUST be zero  
    unsigned char cc;                   //!< CSRC count, normally 0 in the absence of RTP mixers           
    unsigned char marker;               //!< Marker bit  
    unsigned char pt;                   //!< 7 bits, Payload Type, dynamically established  
    unsigned short seq_no;                //!< RTP sequence number, incremented by one for each sent packet   
    unsigned int timestamp;        //!< timestamp, 27 MHz for H.264  
    unsigned int ssrc;             //!< Synchronization Source, chosen randomly  
    unsigned char * payload;      //!< the payload including payload headers  
    unsigned int paylen;           //!< length of payload in bytes  
} RTPpacket_t;

/*
0                 1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC   |M|     PT      |       sequence number         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           timestamp                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           synchronization source (SSRC) identifier            |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|            contributing source (CSRC) identifiers             |
|                             ....                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Be careful with the endianness

intel cpu is little endian, but network byte order is big endian

intel cpu: high -> csrc_len:4 -> extension:1-> padding:1 -> version:2 -> low
in memory:
low  -> 4001(memory address) version:2
|       4002(memory address) padding:1
|       4003(memory address) extension:1
high -> 4004(memory address) csrc_len:4

network byte order: high ->version:2 -> padding:1 -> extension:1 -> csrc_len:4-> low
when saved into memroy:
low  -> 4001(memory address) version:2
|       4002(memory address) padding:1
|       4003(memory address) extension:1
high -> 4004(memory address) csrc_len:4

local memory: high -> csrc_len:4 -> extension:1 -> padding:1 -> version:2 -> low
*/

typedef struct
{
    unsigned char forbidden_bit;           //! Should always be FALSE  
    unsigned char nal_reference_idc;       //! NALU_PRIORITY_xxxx  
    unsigned char nal_unit_type;           //! NALU_TYPE_xxxx    
    unsigned int startcodeprefix_len;      //! 前缀字节�? 
    unsigned int len;                      //! 包含nal 头的nal 长度，从第一�?0000001到下一�?00000001的长�? 
    unsigned int max_size;                 //! 做多一个nal 的长�? 
    unsigned char * buf;                   //! 包含nal 头的nal 数据  
    unsigned int lost_packets;             //! 预留  
} NALU_t;

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |
+---------------+
*/
typedef struct
{
    //byte 0  
    unsigned char TYPE : 5;
    unsigned char NRI : 2;
    unsigned char F : 1;
} NALU_HEADER; // 1 BYTE   

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |
+---------------+
*/
typedef struct
{
    //byte 0  
    unsigned char TYPE : 5;
    unsigned char NRI : 2;
    unsigned char F : 1;
} FU_INDICATOR; // 1 BYTE   

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|S|E|R|  Type   |
+---------------+
*/
typedef struct
{
    //byte 0  
    unsigned char TYPE : 5;
    unsigned char R : 1;
    unsigned char E : 1;
    unsigned char S : 1;
} FU_HEADER;   // 1 BYTES   

#define  MAXDATASIZE 1500  
#define PORT    1234  
#define BUFFER_SIZE 10   

NALU_t *AllocNALU(int buffersize)
{
    NALU_t *n;

    if((n = (NALU_t*)calloc(1, sizeof(NALU_t))) == NULL)
    {
        printf("AllocNALU Error: Allocate Meory To NALU_t Failed ");
        exit(0);
    }
    return n;
}

void FreeNALU(NALU_t *n)
{
    if(n)
    {
        free(n);
    }
}

/*
*bufIn:rtppackage
*len: the lengthe of rtppackage
*/
void rtp_unpackage(char* bufIn, int len, int ID, bool* nfirst)
{

    unsigned char poutfile[1600];
    RTPpacket_t *p = NULL;
    NALU_HEADER * nalu_hdr = NULL;
    NALU_t * n = NULL;
    FU_INDICATOR    *fu_ind = NULL;
    FU_HEADER       *fu_hdr = NULL;
    int total_bytes = 0;                 //当前包传出的数据  
    static int total_recved = 0;         //一共传输的数据  
    int fwrite_number = 0;               //存入文件的数据长�? 

    //memcpy(recvbuf,bufIn, len);          //复制rtp�? 

    //printf("包长�? rtp头：   = %d\n",len);  

    //////////////////////////////////////////////////////////////////////////  
    //begin rtp_payload and rtp_header  

    //p = (RTPpacket_t*)&recvbuf[0];
    p = (RTPpacket_t*)malloc(sizeof(RTPpacket_t));
    if(NULL == p)
    {
        MessageBox(NULL, L"malloc error", NULL, MB_OK);
    }

    p->payload = (unsigned char*)malloc(MAXDATASIZE);
    if(NULL == p)
    {
        MessageBox(NULL, L"malloc error", NULL, MB_OK);
    }

    /*
    0                 1                   2                   3
    0 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |V=2|P|X|  CC   |M|     PT      |       sequence number         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           timestamp                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           synchronization source (SSRC) identifier            |
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    |            contributing source (CSRC) identifiers             |
    |                             ....                              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */

    p->version = ((unsigned char)bufIn[0]) >> 6;

    p->padding = ((unsigned char)bufIn[0]) << 2 >> 7;

    p->extension = ((unsigned char)bufIn[0]) << 3 >> 7;

    p->cc = ((unsigned char)bufIn[0]) << 4 >> 4;

    p->marker = ((unsigned char)bufIn[1]) >> 7;

    p->pt = ((unsigned char)bufIn[1]) << 1 >> 1;

    ((unsigned char*)&(p->seq_no))[0] = bufIn[3];
    ((unsigned char*)&(p->seq_no))[1] = bufIn[2];

    ((unsigned char*)&(p->timestamp))[0] = bufIn[11];
    ((unsigned char*)&(p->timestamp))[1] = bufIn[10];
    ((unsigned char*)&(p->timestamp))[2] = bufIn[9];
    ((unsigned char*)&(p->timestamp))[3] = bufIn[8];
    ((unsigned char*)&(p->timestamp))[4] = bufIn[7];
    ((unsigned char*)&(p->timestamp))[5] = bufIn[6];
    ((unsigned char*)&(p->timestamp))[6] = bufIn[5];
    ((unsigned char*)&(p->timestamp))[7] = bufIn[4];

    p->ssrc = ((RTP_FIXED_HEADER*)&bufIn[0])->ssrc;

#ifdef _DEBUG
    FILE* pFileseq_no = fopen("seq_no.txt", "ab");
    FILE* pFiletime = fopen("timestamp.txt", "ab");
    char* strBuffer = new char[256];

    sprintf(strBuffer, "seq_no: %u\n", p->seq_no);
    fwrite(strBuffer, strlen(strBuffer), 1, pFileseq_no);

    sprintf(strBuffer, "timestamp: %u\n", p->timestamp);
    fwrite(strBuffer, strlen(strBuffer), 1, pFiletime);

    fclose(pFileseq_no);
    fclose(pFiletime);
    delete[] strBuffer;
#endif // !_DEBUG


    //end rtp_payload and rtp_header  
    //////////////////////////////////////////////////////////////////////////  
    //begin nal_hdr
    n = AllocNALU(4096);
    if(NULL == n)          //为结构体nalu_t及其成员buf分配空间。返回值为指向nalu_t存储空间的指�? 
    {
        MessageBox(NULL, L"AllocNALU error", NULL, MB_OK);
    }

    nalu_hdr = (NALU_HEADER*)&bufIn[12];                        //网络传输过来的字节序 ，当存入内存还是和文档描述的相反，只要匹配网络字节序和文档描述即可传输正确�? 
    //printf("forbidden_zero_bit: %d\n",nalu_hdr->F);              //网络传输中的方式为：F->NRI->TYPE.. 内存中存储方式为 TYPE->NRI->F (和nal头匹�?�? 
    n->forbidden_bit = nalu_hdr->F << 7;                          //内存中的字节序�? 
    // printf("nal_reference_idc:  %d\n",nalu_hdr->NRI);  
    n->nal_reference_idc = nalu_hdr->NRI << 5;
    //printf("nal 负载类型:       %d\n",nalu_hdr->TYPE);  
    n->nal_unit_type = nalu_hdr->TYPE;

    //end nal_hdr  
    //////////////////////////////////////////////////////////////////////////  
    //开始解�? 
    if(nalu_hdr->TYPE != 7 && (*nfirst))  //不是67开头的包，并且还是第一个包
    {
        return;
    }
    *nfirst = false;
    if(nalu_hdr->TYPE > 0 && nalu_hdr->TYPE < 24)  //单包  
    {
        poutfile[0] = 0x00;
        poutfile[1] = 0x00;
        poutfile[2] = 0x00;
        poutfile[3] = 0x01;
        //写进起始字节0x00000001  
        total_bytes += 4;
        memcpy(p->payload, &bufIn[13], len - 13);
        p->paylen = len - 13;
        memcpy(poutfile + 4, nalu_hdr, 1);  //写NAL_HEADER  
        total_bytes += 1;
        memcpy(poutfile + 5, p->payload, p->paylen);//写NAL数据
        total_bytes += p->paylen;
        poutfile[total_bytes] = '\0';
    }

    else if(nalu_hdr->TYPE == 28)                    //FU-A分片包，解码顺序和传输顺序相�? 
    {

        fu_ind = (FU_INDICATOR*)&bufIn[12];     //分片包用的是FU_INDICATOR而不是NALU_HEADER  
        n->forbidden_bit = fu_ind->F << 7;
        n->nal_reference_idc = fu_ind->NRI << 5;

        n->nal_unit_type = fu_ind->TYPE;

        fu_hdr = (FU_HEADER*)&bufIn[13];        //FU_HEADER赋�? 
        n->nal_unit_type = fu_hdr->TYPE;               //应用的是FU_HEADER的TYPE  

        if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 1)                      //分片包最后一个包  
        {
            memcpy(p->payload, &bufIn[14], len - 14);
            p->paylen = len - 14;
            memcpy(poutfile, p->payload, p->paylen);  //写NAL数据  
            total_bytes = p->paylen;
            poutfile[total_bytes] = '\0';
        }
        else if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 0)                 //分片�?但不是最后一个包  
        {
            if(fu_hdr->S == 1)                        //分片的第一个包  
            {
                unsigned char F;
                unsigned char NRI;
                unsigned char TYPE;
                unsigned char nh;

                poutfile[0] = 0x00;
                poutfile[1] = 0x00;
                poutfile[2] = 0x00;
                poutfile[3] = 0x01;               //写起始字节码0x00000001  
                total_bytes += 4;

                F = fu_ind->F << 7;
                NRI = fu_ind->NRI << 5;
                TYPE = fu_hdr->TYPE;                                            //应用的是FU_HEADER的TYPE  
                //nh = n->forbidden_bit|n->nal_reference_idc|n->nal_unit_type;  //二进制文件也是按 大字节序存储  
                nh = F | NRI | TYPE;

                poutfile[4] = nh;              //写NAL HEADER  

                total_bytes += 1;
                memcpy(p->payload, &bufIn[14], len - 14);
                p->paylen = len - 14;
                memcpy(poutfile + 5, p->payload, p->paylen);  //写NAL数据  
                total_bytes += p->paylen;
                poutfile[total_bytes] = '\0';
            }
            else                                      //如果不是第一个包  
            {
                memcpy(p->payload, &bufIn[14], len - 14);
                p->paylen = len - 14;
                memcpy(poutfile, p->payload, p->paylen);  //写NAL数据  
                total_bytes = p->paylen;
                poutfile[total_bytes] = '\0';
            }
        }
    }
    else if(nalu_hdr->TYPE == 29)                //FU-B分片包，解码顺序和传输顺序相�? 
    {
        if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 1)                  //分片包最后一个包  
        {
        }
        else if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 0)             //分片�?但不是最后一个包  
        {
        }
    }
    else
    {
    }
    /*total_recved += total_bytes;
    printf("total_recved = %d\n",total_recved);  */

    //memset(recvbuf,0,1500);  
    free(p->payload);
    free(p);
    FreeNALU(n);

    fDecodeVideo DecodeVideo;

    HMODULE hdll = LoadLibrary(L"PlayH264DLL.dll");
    if(NULL == hdll)
    {
        MessageBox(NULL, L"LoadLibrary PlayH264DLL.dll error", L"", MB_OK);
        return;
    }
    DecodeVideo = (fDecodeVideo)GetProcAddress(hdll, "DecodeVideo");
    DecodeVideo(ID, poutfile, total_bytes);

    return;
}
int ftyp = 0;
int moov = 0;


void rtp_unpackage_mpeg(char *bufIn, int len, int ID, bool *nfirst)
{



    unsigned char recvbuf[1500];
    memcpy(recvbuf, bufIn, len);

    if(*nfirst)
    {
        if(!(recvbuf[12] == 0x00 && recvbuf[13] == 0x00 && recvbuf[14] == 0x01 && recvbuf[15] == 0xB0))
            return;
        *nfirst = false;
    }

    fDecodeVideo DecodeVideo;

    HMODULE hdll = LoadLibrary(L"PlayH264DLL.dll");
    if(NULL == hdll)
    {
        MessageBox(NULL, L"LoadLibrary PlayH264DLL.dll error", L"", MB_OK);
        return;
    }
    DecodeVideo = (fDecodeVideo)GetProcAddress(hdll, "DecodeVideo");
    DecodeVideo(ID, recvbuf + 12, len - 12);
}

