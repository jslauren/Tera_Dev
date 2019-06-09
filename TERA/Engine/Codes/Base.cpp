#include "..\Headers\Base.h"

_USING(Engine)

CBase::CBase()
{
}

_ulong CBase::AddRef()
{
	return _ulong(++m_dwRefCnt);
}

unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}



