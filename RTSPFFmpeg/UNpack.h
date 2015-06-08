#pragma once
#ifdef _DEBUG
#include <cstdio>
#endif

#include <Windows.h>

/*

The RTP header has the following format:

|0                 1                   2                   3    |
|0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7|
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
    unsigned char CC : 4;
    unsigned char X : 1;
    unsigned char P : 1;
    unsigned char V : 2;
}RTP_header_byte_0;

typedef struct
{
    unsigned char PT : 7;
    unsigned char M : 1;
}RTP_header_byte_1;

typedef struct
{
    unsigned char  version;                   // V: Version, 2 bits, MUST be 0x2
    unsigned char  padding;                   // P: Padding bit, Padding MUST NOT be used
    unsigned char  extension;                 // X: Extension, MUST be zero
    unsigned char  contributing_source_count; // CC: CSRC count, normally 0 in the absence of RTP mixers
    unsigned char  marker;                    // M: Marker bit
    unsigned char  payload_type;              // PT: 7 bits, Payload Type, dynamically established
    unsigned short sequence_number;           // sequence number: increaced by one for each sent packet
    unsigned int   timestamp;                 // timestamp: 27 MHz for H.264
    unsigned int   synchronization_source;    // synchronization source (SSRC) identifier: chosen randomly
    unsigned int*  contributing_source_list;  // contributing source (CSRC) identifiers: pointer to the array head
}RTP_header;

/*

Summary of NAL unit types and the corresponding packet types

NAL Unit Type | Packet Type | Packet Type Name
--------------+-------------+---------------------------------
0             | reserved    |
1 - 23        | NAL unit    | Single NAL unit packet
24            | STAP - A    | Single - time aggregation packet
25            | STAP - B    | Single - time aggregation packet
26            | MTAP16      | Multi - time aggregation packet
27            | MTAP24      | Multi - time aggregation packet
28            | FU - A      | Fragmentation unit
29            | FU - B      | Fragmentation unit
30 - 31       | reserved    |

*/




/*

Example of NRI values for coded slices and coded slice
data partitions of primary coded reference pictures

NAL Unit Type | Content of NAL Unit          | NRI(binary)
--------------+------------------------------+------------
1             | non - IDR coded slice        | 10
2             | Coded slice data partition A | 10
3             | Coded slice data partition B | 01
4             | Coded slice data partition C | 01

*/

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

typedef struct
{
    // A value of 0 indicates that the NAL unit
    // type octet and payload should not contain bit errors or other
    // syntax violations.A value of 1 indicates that the NAL unit
    // type octet and payload may contain bit errors or other syntax
    // violations.
    unsigned char  forbidden_zero_bit;
    unsigned char  NAL_reference_idc;   // NALU_PRIORITY_xxxx
    unsigned char  NAL_unit_type;       // NALU_TYPE_xxxx
    unsigned int   startcodeprefix_len; // prefix bytes
    unsigned int   len;                 // including nal header's length, from first 00000001 to next 000000001
    unsigned int   max_size;            // make more nal's length
} NALU_t;


class CRTPPackage
{
private:
    // need to be initial by user begin
    unsigned char* m_p_buffer_current_position;
    unsigned char* m_p_buffer_head;
    int m_buffer_size;
    // need to be initial by user end
public:
    CRTPPackage();
    ~CRTPPackage();
    void set_RTP_package_buffer(unsigned char* p_buffer, int buffer_size);
    void unpack_RTP_header(void);
    void unpack_H264_NAL_header(bool* bFirst);
    void unpack_MPEG(bool* bFirst);

    unsigned char* m_p_unpack_result;
    int m_unpack_result_size;

    RTP_header* m_p_RTP_header;
    NALU_t* m_p_NALU_t;
};

