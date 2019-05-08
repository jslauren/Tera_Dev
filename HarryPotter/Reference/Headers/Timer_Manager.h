#pragma once
#include "Base.h"

// 타이머들을 여러개 모아놓은 매니져클래스.
// 타이머를 여러개 만드는 이유 : 
// 1. 여러개의 메인게임이 존재할 수 있기 때문에. 

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

