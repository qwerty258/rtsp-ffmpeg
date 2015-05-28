#include "UNpack.h"

NALU_t* AllocNALU(int buffersize)
{
    NALU_t* n = (NALU_t*)calloc(1, sizeof(NALU_t));

    if(NULL == n)
    {
        MessageBox(NULL, L"AllocNALU Error: Allocate Meory To NALU_t Failed", NULL, MB_OK);
        exit(-1);
    }
    return n;
}

void FreeNALU(NALU_t* n)
{
    if(NULL != n)
    {
        free(n);
    }
}

/*
buffer: rtppackage buffer
len: the length of rtppackage
*/
void RTP_unpackage(char* RTP_package_buffer, int RTP_package_length, int ID, bool* nfirst)
{
    char* current_buffer_position = RTP_package_buffer;

    unsigned char poutfile[1600];
    //NALU_HEADER * nalu_hdr = NULL;
    NALU_t* n = NULL;
    FU_INDICATOR    *fu_ind = NULL;
    FU_HEADER       *fu_hdr = NULL;
    int total_bytes = 0;                 // current package data
    static int total_recved = 0;         // total received data
    int fwrite_number = 0;               // bytes written to file

    //begin rtp_payload and rtp_header

    RTP_header* p_RTP_header = (RTP_header*)calloc(1, sizeof(RTP_header));
    if(NULL == p_RTP_header)
    {
        MessageBox(NULL, L"memory calloc error", NULL, MB_OK);
        exit(-1);
    }

    p_RTP_header->payload = (unsigned char*)calloc(1, MAXDATASIZE);
    if(NULL == p_RTP_header->payload)
    {
        MessageBox(NULL, L"memory calloc error", NULL, MB_OK);
        exit(-1);
    }

    // byte 0:
    p_RTP_header->version = ((RTP_header_byte_0*)current_buffer_position)->V;
    p_RTP_header->padding = ((RTP_header_byte_0*)current_buffer_position)->P;
    p_RTP_header->extension = ((RTP_header_byte_0*)current_buffer_position)->X;
    p_RTP_header->contributing_source_count = ((RTP_header_byte_0*)current_buffer_position)->CC;

    current_buffer_position += 1;

    // byte 1:
    p_RTP_header->marker = ((RTP_header_byte_1*)current_buffer_position)->M;
    p_RTP_header->payload_type = ((RTP_header_byte_1*)current_buffer_position)->PT;

    current_buffer_position += 1;

    // byte 2-3: sequence number
    ((unsigned char*)&(p_RTP_header->sequence_number))[0] = current_buffer_position[1];
    ((unsigned char*)&(p_RTP_header->sequence_number))[1] = current_buffer_position[0];

    current_buffer_position += 2;

    // byte 4-7: timestamp
    ((unsigned char*)&(p_RTP_header->timestamp))[0] = (unsigned char)current_buffer_position[3];
    ((unsigned char*)&(p_RTP_header->timestamp))[1] = (unsigned char)current_buffer_position[2];
    ((unsigned char*)&(p_RTP_header->timestamp))[2] = (unsigned char)current_buffer_position[1];
    ((unsigned char*)&(p_RTP_header->timestamp))[3] = (unsigned char)current_buffer_position[0];

    current_buffer_position += 4;

    // byte 8-11: synchronization source (SSRC) identifier
    ((unsigned char*)&(p_RTP_header->synchronization_source))[0] = (unsigned char)current_buffer_position[3];
    ((unsigned char*)&(p_RTP_header->synchronization_source))[1] = (unsigned char)current_buffer_position[2];
    ((unsigned char*)&(p_RTP_header->synchronization_source))[2] = (unsigned char)current_buffer_position[1];
    ((unsigned char*)&(p_RTP_header->synchronization_source))[3] = (unsigned char)current_buffer_position[0];

    current_buffer_position += 4;

    // contributing source (CSRC) identifiers
    if(0 != p_RTP_header->contributing_source_count)
    {
        p_RTP_header->contributing_source_list = (unsigned int*)calloc(1, (p_RTP_header->contributing_source_count) * sizeof(unsigned int));
        if(NULL == p_RTP_header->contributing_source_list)
        {
            MessageBox(NULL, L"calloc error", NULL, MB_OK);
            exit(-1);
        }
        for(unsigned char i = 0; i < p_RTP_header->contributing_source_count; ++i)
        {
            ((unsigned char*)&p_RTP_header->contributing_source_list[i])[0] = current_buffer_position[3];
            ((unsigned char*)&p_RTP_header->contributing_source_list[i])[1] = current_buffer_position[2];
            ((unsigned char*)&p_RTP_header->contributing_source_list[i])[2] = current_buffer_position[1];
            ((unsigned char*)&p_RTP_header->contributing_source_list[i])[3] = current_buffer_position[0];

            current_buffer_position += 4;
        }
    }

#ifdef _DEBUG
    FILE* p_file_sequence_number = fopen("sequence_number.txt", "ab");
    FILE* p_file_timestamp = fopen("timestamp.txt", "ab");
    char* strBuffer = new char[256];

    sprintf(strBuffer, "%05u\n", p_RTP_header->sequence_number);
    fwrite(strBuffer, strlen(strBuffer), 1, p_file_sequence_number);

    sprintf(strBuffer, "%010u\n", p_RTP_header->timestamp);
    fwrite(strBuffer, strlen(strBuffer), 1, p_file_timestamp);

    fclose(p_file_sequence_number);
    fclose(p_file_timestamp);
    delete[] strBuffer;
#endif // !_DEBUG


    //end rtp_payload and rtp_header
    //////////////////////////////////////////////////////////////////////////  
    //begin nal_hdr
    n = AllocNALU(4096);
    if(NULL == n)          // locate memory for nalu_t and it's members
    {
        MessageBox(NULL, L"memory AllocNALU error", NULL, MB_OK);
        exit(-1);
    }

    //nalu_hdr = (NALU_HEADER*)current_buffer_position; //网络传输过来的字节序 ，当存入内存还是和文档描述的相反，只要匹配网络字节序和文档描述即可传输正确�? 
    //printf("forbidden_zero_bit: %d\n",nalu_hdr->F);              //网络传输中的方式为：F->NRI->TYPE.. 内存中存储方式为 TYPE->NRI->F (和nal头匹�?�? 
    n->forbidden_zero_bit = ((NALU_HEADER*)current_buffer_position)->F; //<< 7;                          //内存中的字节序�? 
    n->NAL_reference_idc = ((NALU_HEADER*)current_buffer_position)->NRI; //<< 5;
    n->NAL_unit_type = ((NALU_HEADER*)current_buffer_position)->TYPE;

    //end nal_hdr  
    //////////////////////////////////////////////////////////////////////////  
    //开始解�? 
    if(((NALU_HEADER*)current_buffer_position)->TYPE != 7 && (*nfirst))  //不是67开头的包，并且还是第一个包
    {
        if(NULL != p_RTP_header->contributing_source_list)
        {
            free(p_RTP_header->contributing_source_list);
            p_RTP_header->contributing_source_list = NULL;
        }
        if(NULL != p_RTP_header->payload)
        {
            free(p_RTP_header->payload);
            p_RTP_header->payload = NULL;
        }
        if(NULL != p_RTP_header)
        {
            free(p_RTP_header);
            p_RTP_header = NULL;
        }
        FreeNALU(n);

        return;
    }
    *nfirst = false;
    if(((NALU_HEADER*)current_buffer_position)->TYPE > 0 && ((NALU_HEADER*)current_buffer_position)->TYPE < 24)  //单包  
    {
        poutfile[0] = 0x00;
        poutfile[1] = 0x00;
        poutfile[2] = 0x00;
        poutfile[3] = 0x01;
        //写进起始字节0x00000001  
        total_bytes += 4;
        memcpy(p_RTP_header->payload, &RTP_package_buffer[13], RTP_package_length - 13);
        p_RTP_header->paylen = RTP_package_length - 13;
        memcpy(poutfile + 4, ((NALU_HEADER*)current_buffer_position), 1);  //写NAL_HEADER  
        total_bytes += 1;
        memcpy(poutfile + 5, p_RTP_header->payload, p_RTP_header->paylen);//写NAL数据
        total_bytes += p_RTP_header->paylen;
        poutfile[total_bytes] = '\0';
    }
    else if(((NALU_HEADER*)current_buffer_position)->TYPE == 28)                    //FU-A分片包，解码顺序和传输顺序相�? 
    {

        fu_ind = (FU_INDICATOR*)&RTP_package_buffer[12];     //分片包用的是FU_INDICATOR而不是NALU_HEADER  
        n->forbidden_zero_bit = fu_ind->F << 7;
        n->NAL_reference_idc = fu_ind->NRI << 5;

        n->NAL_unit_type = fu_ind->TYPE;

        fu_hdr = (FU_HEADER*)&RTP_package_buffer[13];        //FU_HEADER赋�? 
        n->NAL_unit_type = fu_hdr->TYPE;               //应用的是FU_HEADER的TYPE  

        if(p_RTP_header->marker == 1)                      //分片包最后一个包  
        {
            memcpy(p_RTP_header->payload, &RTP_package_buffer[14], RTP_package_length - 14);
            p_RTP_header->paylen = RTP_package_length - 14;
            memcpy(poutfile, p_RTP_header->payload, p_RTP_header->paylen);  //写NAL数据  
            total_bytes = p_RTP_header->paylen;
            poutfile[total_bytes] = '\0';
        }
        else if(p_RTP_header->marker == 0)                 //分片�?但不是最后一个包  
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
                memcpy(p_RTP_header->payload, &RTP_package_buffer[14], RTP_package_length - 14);
                p_RTP_header->paylen = RTP_package_length - 14;
                memcpy(poutfile + 5, p_RTP_header->payload, p_RTP_header->paylen);  //写NAL数据  
                total_bytes += p_RTP_header->paylen;
                poutfile[total_bytes] = '\0';
            }
            else                                      //如果不是第一个包  
            {
                memcpy(p_RTP_header->payload, &RTP_package_buffer[14], RTP_package_length - 14);
                p_RTP_header->paylen = RTP_package_length - 14;
                memcpy(poutfile, p_RTP_header->payload, p_RTP_header->paylen);  //写NAL数据  
                total_bytes = p_RTP_header->paylen;
                poutfile[total_bytes] = '\0';
            }
        }
    }
    else if(((NALU_HEADER*)current_buffer_position)->TYPE == 29)                //FU-B分片包，解码顺序和传输顺序相�? 
    {
        //if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 1)                  //分片包最后一个包  
        //{
        //}
        //else if(((RTP_FIXED_HEADER*)&bufIn[0])->marker == 0)             //分片�?但不是最后一个包  
        //{
        //}
    }

    if(NULL != p_RTP_header->contributing_source_list)
    {
        free(p_RTP_header->contributing_source_list);
        p_RTP_header->contributing_source_list = NULL;
    }
    if(NULL != p_RTP_header->payload)
    {
        free(p_RTP_header->payload);
        p_RTP_header->payload = NULL;
    }
    if(NULL != p_RTP_header)
    {
        free(p_RTP_header);
        p_RTP_header = NULL;
    }
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


void RTP_unpackage_mpeg(char *bufIn, int len, int ID, bool *nfirst)
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

