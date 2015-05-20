#ifndef __T_LOCK_H__
#define __T_LOCK_H__

class TLock
{
public:
	TLock(void)
	{
		m_pCS = new CRITICAL_SECTION();
		InitializeCriticalSection(m_pCS);
	};

	virtual ~TLock(void)
	{
		DeleteCriticalSection(m_pCS);
		delete m_pCS;
	};

	virtual void Lock(void)
	{
		EnterCriticalSection(m_pCS);
	};

	virtual void UnLock(void)
	{
		LeaveCriticalSection(m_pCS);
	};

protected:
	CRITICAL_SECTION* m_pCS;
};

class ATLock
{
public:
	ATLock(TLock* pTLock)
	{
		m_pTLock = pTLock;

		m_pTLock->Lock();
	};

	ATLock(TLock& lock)
	{
		m_pTLock = &lock;

		m_pTLock->Lock();
	};


	virtual ~ATLock()
	{
		m_pTLock->UnLock();
	};

protected:

	TLock* m_pTLock;
};

#endif //__T_LOCK_H__