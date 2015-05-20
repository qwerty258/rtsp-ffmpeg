// VBuffer.h: interface for the buffer vector class.
// Author:			ChenRui
// Contact:			ChenRui@Sobey.com
// LastUpdteDate:	2008-05-23
//
// How to use the VBuffer:
//  
//	[heard] ... FullBuffer  FullBuffer  FullBuffer  ... [tail]
//
//	GetFullBuffer()							        AddFullBuffer()
//
//	   ↓                                                  ↑	
//
//	AddEmptyBuffer()					            GetEmptyBuffer()
//
//	[tail]  ... EmptyBuffer EmptyBuffer EmptyBuffer ... [heard]
//
//
//  Split the VBuffer:
//
//	SetData() → [tail]  ... Buffer Buffer ... [heard]   → GetData()
//
//
//////////////////////////////////////////////////////////////////////

#ifndef __V_BUFFER_H__
#define __V_BUFFER_H__

#include <list>
using namespace std;

struct Buffer
{
	BYTE*	 m_pBuffer;
	UINT	 m_nBufferSize;

	BYTE*	 m_pData;
	UINT	 m_nDataSize;
	
	Buffer()
	{
		m_pBuffer = NULL;
		m_nBufferSize = 0;

		m_pData = NULL;
		m_nDataSize = 0;
	};

	virtual ~Buffer()
	{
		FreeBuffer();
	};

	virtual void FreeBuffer()
	{
		if (m_pBuffer == NULL)
			return;

		delete[] m_pBuffer;
		
		m_pBuffer = NULL;
		m_nBufferSize = 0;

		m_pData = NULL;
		m_nDataSize = 0;
	};

	virtual BOOL AllocateBuffer(UINT nBufferSize)
	{
		if ( nBufferSize == 0)
			return FALSE;

		FreeBuffer();
		
		m_pBuffer = new BYTE[nBufferSize];
		if ( m_pBuffer == NULL)
			return FALSE;
		m_nBufferSize = nBufferSize;

		m_pData = m_pBuffer;

		return true;
	};

	virtual BOOL ExtendBuffer(UINT nBufferSize)
	{
		if (nBufferSize <= m_nBufferSize) 
			return TRUE;

		return AllocateBuffer(nBufferSize);
	};

	virtual BOOL FillData(PBYTE pData, UINT nDataSize)
	{
		if (pData == NULL || nDataSize == 0) 
			return FALSE;

		if (m_nBufferSize < nDataSize)
			AllocateBuffer(nDataSize);
		
		memcpy(m_pBuffer, pData, nDataSize);
		m_nDataSize = nDataSize;

		return TRUE;
	};

	virtual BOOL AppendData(PBYTE pData, UINT nDataSize)
	{
		if (pData == NULL || nDataSize == 0) 
			return FALSE;

		if (!m_pBuffer && !m_nBufferSize)
			AllocateBuffer(nDataSize);

		if ( ( m_nBufferSize - ( m_pData - m_pBuffer)) < ( m_nDataSize + nDataSize))
		{
			Buffer bufferTemp;
			if (m_nDataSize)
			{
				if ( bufferTemp.FillData(m_pData, m_nDataSize) == FALSE)
					return FALSE;
			}

			PBYTE pBuffer = NULL;
			pBuffer = new BYTE[m_nDataSize + nDataSize];
			if ( pBuffer == NULL)
				return FALSE;

			if ( m_pBuffer != NULL)
				delete [] m_pBuffer;

			m_pBuffer = pBuffer;
			m_pData = pBuffer;
			
			m_nBufferSize = m_nDataSize + nDataSize;

			if (bufferTemp.m_nDataSize)
				memcpy( m_pData, bufferTemp.m_pData, bufferTemp.m_nDataSize);
			m_nDataSize = bufferTemp.m_nDataSize;
		}
		
		memcpy( m_pData + m_nDataSize, pData, nDataSize);
		m_nDataSize += nDataSize;

		return TRUE;
	};

	virtual void ClearData()
	{
		m_nDataSize = 0;
		m_pData = m_pBuffer;
	};
};

class VBuffer
{
public:
	VBuffer()
	{
		m_nDataSize = 0;
		m_nEmptyBufferSize = 0;
		m_nFullBufferSize = 0;
	};

	virtual ~VBuffer()
	{
		FreeBuffer();
	};

	virtual BOOL AllocateBuffer(UINT nBufferCount = 100, UINT nBufferSize = 1024*1024)
	{
		if (nBufferCount <= 0 || nBufferSize <= 0) 
			return FALSE;

		FreeBuffer();

		for (UINT iBuffer = 0; iBuffer < nBufferCount; iBuffer++)
		{
			Buffer* pBuffer = new Buffer();

			if ( pBuffer->AllocateBuffer(nBufferSize) == FALSE)
			{
				delete pBuffer;
				return FALSE;
			}

			m_EmptyBuffer.push_back(pBuffer);

			m_nEmptyBufferSize += nBufferSize;
		}

		return TRUE;
	};

	virtual void FreeBuffer()
	{
		list<Buffer*>::iterator iFullBuffer = m_FullBuffer.begin();
		for ( ; iFullBuffer != m_FullBuffer.end(); iFullBuffer++ )
		{
			delete *iFullBuffer;
		}
		m_FullBuffer.clear();
		
		m_nFullBufferSize = 0;
		m_nDataSize = 0;

		list<Buffer*>::iterator iEmptyBuffer = m_EmptyBuffer.begin();
		for ( ; iEmptyBuffer != m_EmptyBuffer.end(); iEmptyBuffer++ )
		{
			delete *iEmptyBuffer;
		}
		m_EmptyBuffer.clear();

		m_nEmptyBufferSize = 0;
	};

	virtual void ResetBuffer()
	{
		list<Buffer*>::iterator iFullBuffer = m_FullBuffer.begin();
		for ( ; iFullBuffer != m_FullBuffer.end(); iFullBuffer++ )
		{   
			m_nDataSize -= (*iFullBuffer)->m_nDataSize;
			m_nFullBufferSize -= (*iFullBuffer)->m_nBufferSize;
			m_nEmptyBufferSize += (*iFullBuffer)->m_nBufferSize;

			(*iFullBuffer)->ClearData();
			m_EmptyBuffer.push_back(*iFullBuffer);
		}

		m_FullBuffer.clear();
	};

	virtual BOOL AddEmptyBuffer(Buffer* pEmptyBuffer)
	{
		if ( pEmptyBuffer == NULL)
			return FALSE;

		pEmptyBuffer->ClearData();

		m_EmptyBuffer.push_back(pEmptyBuffer);
		
		m_nEmptyBufferSize += pEmptyBuffer->m_nBufferSize;

		return TRUE;
	};


	virtual BOOL AddFullBuffer(Buffer* pFullBuffer)
	{
		if ( pFullBuffer == NULL)
			return FALSE;

		m_FullBuffer.push_back(pFullBuffer);

		m_nDataSize += pFullBuffer->m_nDataSize;
		m_nFullBufferSize += pFullBuffer->m_nBufferSize;

		return TRUE;
	};

	virtual Buffer* GetEmptyBuffer()
	{
		if( m_EmptyBuffer.empty())
			return NULL;
		
		Buffer* pEmptyBuffer = m_EmptyBuffer.front();
		m_EmptyBuffer.pop_front();
		m_nEmptyBufferSize -= pEmptyBuffer->m_nBufferSize;

		return pEmptyBuffer;
	};

	virtual Buffer* GetFullBuffer()
	{
		if( m_FullBuffer.empty())
			return NULL;

		Buffer* pFullBuffer = m_FullBuffer.front();
		m_FullBuffer.pop_front();
		m_nDataSize -= pFullBuffer->m_nDataSize;
		m_nFullBufferSize -= pFullBuffer->m_nBufferSize;

		return pFullBuffer;
	};

	virtual UINT GetFullBufferCount()
	{
		return m_FullBuffer.size();
	};

	virtual UINT GetEmptyBufferCount()
	{
		return m_EmptyBuffer.size();
	};
	
	virtual ULONG GetDataSize()
	{
		return m_nDataSize;
	};

	virtual ULONG GetBufferSize()
	{
		return m_nEmptyBufferSize + m_nFullBufferSize;
	};

	virtual ULONG GetEmptyBufferSize()
	{
		return m_nEmptyBufferSize;
	};

	virtual ULONG GetFullBufferSize()
	{
		return m_nFullBufferSize;
	};

	virtual BOOL GetData(BYTE* pData, UINT32 nDataSize)
	{
		if (pData == NULL || nDataSize == 0)
			return FALSE;

		if (m_nDataSize < nDataSize) 
			return FALSE;

		Buffer* pBuffer;
		UINT	nGetSize = 0;
		UINT	nCopyDataSize;		//	获取部分长度
		
		while(nGetSize < nDataSize)	//	获取至足够的数据后返回
		{
			pBuffer = m_FullBuffer.front();

			if ( nDataSize - nGetSize < pBuffer->m_nDataSize)
			{
				// 只取buffer中部分数据进行拷贝
				nCopyDataSize = nDataSize - nGetSize;	//	拷贝部分长度

				memcpy(pData + nGetSize, pBuffer->m_pData, nCopyDataSize);
				
				// 修正可用数据偏移
				pBuffer->m_pData += nCopyDataSize;
				pBuffer->m_nDataSize -= nCopyDataSize;
			}
			else
			{
				nCopyDataSize = pBuffer->m_nDataSize;	//	拷贝部分长度

				memcpy(pData + nGetSize, pBuffer->m_pData, nCopyDataSize);

				// 移送buffer到空缓冲区中
				m_nFullBufferSize -= pBuffer->m_nBufferSize;
				m_nEmptyBufferSize += pBuffer->m_nBufferSize;

				pBuffer->ClearData();
				m_EmptyBuffer.push_back(pBuffer);
				m_FullBuffer.pop_front();
			}

			nGetSize += nCopyDataSize;
			m_nDataSize -= nCopyDataSize;
		}
		return TRUE;
	}

	virtual BOOL SetData(BYTE* pData, UINT nDataSize)
	{
		if (pData == NULL || nDataSize == 0)
			return FALSE;

		if ( m_nEmptyBufferSize < nDataSize) 
			return FALSE;

		Buffer* pBuffer;
		UINT	nFillSize;
		UINT	nCopyDataSize;

		nFillSize = 0;

		while(nFillSize < nDataSize)	//	填充至足够的数据后返回
		{
			pBuffer = m_EmptyBuffer.front();
			pBuffer->ClearData();

			if (nDataSize - nFillSize < pBuffer->m_nBufferSize)
			{	
				nCopyDataSize = nDataSize - nFillSize;		//	拷贝部分长度
				
				memcpy(pBuffer->m_pBuffer, pData + nFillSize, nCopyDataSize);
				pBuffer->m_nDataSize = nCopyDataSize;
			}
			else
			{
				nCopyDataSize = pBuffer->m_nBufferSize;

				memcpy(pBuffer->m_pBuffer, pData + nFillSize, nCopyDataSize);
				pBuffer->m_nDataSize = nCopyDataSize;
			}
			nFillSize += nCopyDataSize;

			// 移送buffer到数据缓冲区中
			m_nDataSize += pBuffer->m_nDataSize;
			m_nFullBufferSize += pBuffer->m_nBufferSize;
			m_nEmptyBufferSize -= pBuffer->m_nBufferSize;
			
			m_FullBuffer.push_back(pBuffer);
			m_EmptyBuffer.pop_front();
		}

		return TRUE;
	}

protected:

	list<Buffer*> m_FullBuffer;
	list<Buffer*> m_EmptyBuffer;

	ULONG	m_nDataSize;
	ULONG	m_nEmptyBufferSize;
	ULONG	m_nFullBufferSize;
};

#endif // !__V_BUFFER_H__
