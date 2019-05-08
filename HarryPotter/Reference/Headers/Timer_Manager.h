#pragma once
#include "Base.h"

// Ÿ�̸ӵ��� ������ ��Ƴ��� �Ŵ���Ŭ����.
// Ÿ�̸Ӹ� ������ ����� ���� : 
// 1. �������� ���ΰ����� ������ �� �ֱ� ������. 

_BEGIN(Engine)

class CTimer;
class _DLL_EXPORTS CTimer_Manager final : public CBase
{
	_DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float	Compute_TimeDelta(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag);
public:
	virtual void Free();
};

_END

