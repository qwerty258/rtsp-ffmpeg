#include "UNpack.h"

CRTPPackage::CRTPPackage()
{
    m_p_buffer_current_position = NULL;

    m_p_buffer_head = NULL;

    m_p_unpack_result = (unsigned char*)malloc(1600);
    if(NULL == m_p_unpack_result)
    {
        MessageBox(NULL, L"memory malloc error", NULL, MB_OK);
    }

    m_p_RTP_header = (RTP_header*)malloc(sizeof(RTP_header));
    if(NULL == m_p_RTP_header)
    {
        MessageBox(NULL, L"memory malloc error", NULL, MB_OK);
    }

    m_p_RTP_header->contributing_source_list = (unsigned int*)malloc(1);
    if(NULL == m_p_RTP_header->contributing_source_list)
    {
        MessageBox(NULL, L"memory malloc error", NULL, MB_OK);
    }

    m_p_NALU_t = (NALU_t*)malloc(sizeof(NALU_t));
    if(NULL == m_p_NALU_t)
    {
        MessageBox(NULL, L"memory malloc error", NULL, MB_OK);
    }
}

CRTPPackage::~CRTPPackage()
{
    if(NULL != m_p_NALU_t)
    {
        free(m_p_NALU_t);
    }

    if(NULL != m_p_RTP_header->contributing_source_list)
    {
        free(m_p_RTP_header->contributing_source_list);
    }

    if(NULL != m_p_RTP_header)
    {
        free(m_p_RTP_header);
    }

    if(NULL != m_p_unpack_result)
    {
        free(m_p_unpack_result);
    }
}

void CRTPPackage::set_RTP_package_buffer(unsigned char* p_buffer, int buffer_size)
{
    if(NULL == p_buffer || 0 > buffer_size)
    {
        return;
    }

    m_p_buffer_current_position = m_p_buffer_head = p_buffer;
    m_buffer_size = buffer_size;
}

void CRTPPackage::unpack_RTP_header(void)
{
    // byte 0:
    m_p_RTP_header->version = ((RTP_header_byte_0*)m_p_buffer_current_position)->V;
    m_p_RTP_header->padding = ((RTP_header_byte_0*)m_p_buffer_current_position)->P;
    m_p_RTP_header->extension = ((RTP_header_byte_0*)m_p_buffer_current_position)->X;
    m_p_RTP_header->contributing_source_count = ((RTP_header_byte_0*)m_p_buffer_current_position)->CC;

    m_p_buffer_current_position += 1;

    // byte 1:
    m_p_RTP_header->marker = ((RTP_header_byte_1*)m_p_buffer_current_position)->M;
    m_p_RTP_header->payload_type = ((RTP_header_byte_1*)m_p_buffer_current_position)->PT;

    m_p_buffer_current_position += 1;

    // byte 2-3: sequence number
    ((unsigned char*)&(m_p_RTP_header->sequence_number))[0] = m_p_buffer_current_position[1];
    ((unsigned char*)&(m_p_RTP_header->sequence_number))[1] = m_p_buffer_current_position[0];

    m_p_buffer_current_position += 2;

    // byte 4-7: timestamp
    ((unsigned char*)&(m_p_RTP_header->timestamp))[0] = (unsigned char)m_p_buffer_current_position[3];
    ((unsigned char*)&(m_p_RTP_header->timestamp))[1] = (unsigned char)m_p_buffer_current_position[2];
    ((unsigned char*)&(m_p_RTP_header->timestamp))[2] = (unsigned char)m_p_buffer_current_position[1];
    ((unsigned char*)&(m_p_RTP_header->timestamp))[3] = (unsigned char)m_p_buffer_current_position[0];

    m_p_buffer_current_position += 4;

    // byte 8-11: synchronization source (SSRC) identifier
    ((unsigned char*)&(m_p_RTP_header->synchronization_source))[0] = (unsigned char)m_p_buffer_current_position[3];
    ((unsigned char*)&(m_p_RTP_header->synchronization_source))[1] = (unsigned char)m_p_buffer_current_position[2];
    ((unsigned char*)&(m_p_RTP_header->synchronization_source))[2] = (unsigned char)m_p_buffer_current_position[1];
    ((unsigned char*)&(m_p_RTP_header->synchronization_source))[3] = (unsigned char)m_p_buffer_current_position[0];

    m_p_buffer_current_position += 4;

    // contributing source (CSRC) identifiers
    if(0 != m_p_RTP_header->contributing_source_count)
    {
        m_p_RTP_header->contributing_source_list = (unsigned int*)realloc(m_p_RTP_header->contributing_source_list, (m_p_RTP_header->contributing_source_count) * sizeof(unsigned int));
        if(NULL == m_p_RTP_header->contributing_source_list)
        {
            MessageBox(NULL, L"realloc error", NULL, MB_OK);
        }

        for(unsigned char i = 0; i < m_p_RTP_header->contributing_source_count; ++i)
        {
            ((unsigned char*)&m_p_RTP_header->contributing_source_list[i])[0] = m_p_buffer_current_position[3];
            ((unsigned char*)&m_p_RTP_header->contributing_source_list[i])[1] = m_p_buffer_current_position[2];
            ((unsigned char*)&m_p_RTP_header->contributing_source_list[i])[2] = m_p_buffer_current_position[1];
            ((unsigned char*)&m_p_RTP_header->contributing_source_list[i])[3] = m_p_buffer_current_position[0];

            m_p_buffer_current_position += 4;
        }
    }

#ifdef _DEBUG
    FILE* p_file_sequence_number = fopen("C:\\sequence_number.log", "ab");
    FILE* p_file_timestamp = fopen("C:\\timestamp.log", "ab");
    char* strBuffer = new char[256];

    sprintf(strBuffer, "%05u\n", m_p_RTP_header->sequence_number);
    fwrite(strBuffer, strlen(strBuffer), 1, p_file_sequence_number);

    sprintf(strBuffer, "%010u\n", m_p_RTP_header->timestamp);
    fwrite(strBuffer, strlen(strBuffer), 1, p_file_timestamp);

    fclose(p_file_sequence_number);
    fclose(p_file_timestamp);
    delete[] strBuffer;
#endif // !_DEBUG

    //end rtp_payload and rtp_header
}

void CRTPPackage::unpack_H264_NAL_header(bool* bFirst)
{
    m_unpack_result_size = 0;
    memset(m_p_unpack_result, 0x0, 1600);

    m_p_NALU_t->forbidden_zero_bit = ((NALU_HEADER*)m_p_buffer_current_position)->F;
    m_p_NALU_t->NAL_reference_idc = ((NALU_HEADER*)m_p_buffer_current_position)->NRI;
    m_p_NALU_t->NAL_unit_type = ((NALU_HEADER*)m_p_buffer_current_position)->TYPE;

    //end NAL unit header

    // not "sequence parameter set" and the first package
    if(7 != m_p_NALU_t->NAL_unit_type && *bFirst)
    {
        return;
    }
    *bFirst = false;

    // Single NAL Unit Packet
    if(0 < m_p_NALU_t->NAL_unit_type && m_p_NALU_t->NAL_unit_type < 24)
    {
        // 0x00000001 for framing
        m_p_unpack_result[0] = 0x0;
        m_p_unpack_result[1] = 0x0;
        m_p_unpack_result[2] = 0x0;
        m_p_unpack_result[3] = 0x1;
        m_unpack_result_size += 4;

        // directly copy header and data to unpack result
        memcpy(m_p_unpack_result + 4, m_p_buffer_current_position, m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
        m_unpack_result_size += (m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
    }
    // Fragmentation Units A (FU-A), decoding order and net transfer order are same
    else if(28 == m_p_NALU_t->NAL_unit_type)
    {
        // the last package of FU-A, directly copy data to unpack result
        if(1 == m_p_RTP_header->marker)
        {
            m_p_buffer_current_position += 2;

            memcpy(m_p_unpack_result, m_p_buffer_current_position, m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
            m_unpack_result_size += (m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
        }
        // FU-A, but not last package
        else if(0 == m_p_RTP_header->marker)
        {
            unsigned char NAL_header_temp = (((FU_INDICATOR*)m_p_buffer_current_position)->F << 7) | (((FU_INDICATOR*)m_p_buffer_current_position)->NRI << 5);
            m_p_buffer_current_position += 1;

            // FU-A and first package, reconstruct NAL Unit header, then put header and data into unpack result
            if(((FU_HEADER*)m_p_buffer_current_position)->S == 1)
            {
                // 0x00000001 for framing
                m_p_unpack_result[0] = 0x0;
                m_p_unpack_result[1] = 0x0;
                m_p_unpack_result[2] = 0x0;
                m_p_unpack_result[3] = 0x1;
                m_unpack_result_size += 4;

                NAL_header_temp |= ((FU_HEADER*)m_p_buffer_current_position)->TYPE;
                m_p_buffer_current_position += 1;

                m_p_unpack_result[4] = NAL_header_temp;
                m_unpack_result_size += 1;

                memcpy(m_p_unpack_result + 5, m_p_buffer_current_position, m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
                m_unpack_result_size += (m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
            }
            // Fragmentation Units A (FU-A) not first package nor last, directly copy data into unpack result
            else
            {
                m_p_buffer_current_position += 1;

                memcpy(m_p_unpack_result, m_p_buffer_current_position, m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
                m_unpack_result_size += (m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
            }
        }
    }
}

void CRTPPackage::unpack_MPEG(bool* bFirst)
{
    m_unpack_result_size = 0;
    memset(m_p_unpack_result, 0x0, 1600);

    if(*bFirst)
    {
        if(!(m_p_buffer_current_position[0] == 0x00 && m_p_buffer_current_position[1] == 0x00 && m_p_buffer_current_position[2] == 0x01 && m_p_buffer_current_position[3] == 0xB0))
            return;
        *bFirst = false;
    }

    memcpy(m_p_unpack_result, m_p_buffer_current_position, m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
    m_unpack_result_size += (m_buffer_size - (m_p_buffer_current_position - m_p_buffer_head));
}