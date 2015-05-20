// VBufferT.h: interface for the buffer vector class on TLock.
// Author:			ChenRui
// Contact:			ChenRui@Sobey.com
// LastUpdteDate:	2008-05-23
//
//////////////////////////////////////////////////////////////////////

#ifndef __V_BUFFER_T_H__
#define __V_BUFFER_T_H__

#include "VBuffer.h"
#include "TLock.h"

class VBufferT : public VBuffer, public TLock
{
public:
	VBufferT()
	{
	};

	virtual ~VBufferT()
	{
	};

	virtual BOOL AllocateBuffer(UINT nBufferCount = 100, UINT nBufferSize = 1024*1024)
	{
		ATLock lock(this);

		return VBuffer::AllocateBuffer(nBufferCount, nBufferSize);
	};

	virtual void FreeBuffer()
	{
		ATLock lock(this);

		VBuffer::FreeBuffer();
	};

	virtual void ResetBuffer()
	{
		ATLock lock(this);

		VBuffer::ResetBuffer();
	};

	virtual BOOL AddEmptyBuffer(Buffer* pEmptyBuffer)
	{
		ATLock lock(this);

		return VBuffer::AddEmptyBuffer(pEmptyBuffer);
	};

	virtual BOOL AddFullBuffer(Buffer* pFullBuffer)
	{
		ATLock lock(this);

		return VBuffer::AddFullBuffer(pFullBuffer);
	};

	virtual Buffer* GetEmptyBuffer()
	{
		ATLock lock(this);

		return VBuffer::GetEmptyBuffer();
	};

	virtual Buffer* GetFullBuffer()
	{
		ATLock lock(this);

		return VBuffer::GetFullBuffer();
	};

	virtual UINT GetFullBufferCount()
	{
		ATLock lock(this);

		return VBuffer::GetFullBufferCount();
	};

	virtual UINT GetEmptyBufferCount()
	{
		ATLock lock(this);

		return VBuffer::GetEmptyBufferCount();
	};
	
	virtual ULONG GetDataSize()
	{
		ATLock lock(this);

		return VBuffer::GetDataSize();
	};

	virtual ULONG GetBufferSize()
	{
		ATLock lock(this);

		return VBuffer::GetBufferSize();
	};

	virtual ULONG GetEmptyBufferSize()
	{
		ATLock lock(this);

		return VBuffer::GetEmptyBufferSize();
	};

	virtual ULONG GetFullBufferSize()
	{
		ATLock lock(this);

		return VBuffer::GetFullBufferSize();
	};

	virtual BOOL GetData(BYTE* pData, UINT nDataSize)
	{
		ATLock lock(this);

		return VBuffer::GetData(pData, nDataSize);
	}

	virtual BOOL SetData(BYTE* pData, UINT nDataSize)
	{
		ATLock lock(this);

		return VBuffer::SetData(pData, nDataSize);
	}

protected:

};

#endif // !__V_BUFFER_T_H__
