#include "..\Headers\Timer_Manager.h"
#include "Timer.h"

_IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);
	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMERS::value_type(pTimerTag, pTimer));

	return NOERROR;
}

_float CTimer_Manager::Compute_TimeDelta(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Compute_TimeDelta();
}

CTimer * CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), [&](MAPTIMERS::value_type Pair)->bool {if (0 == lstrcmp(pTimerTag, Pair.first)) return true; return false; });

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& Pair : m_mapTimers)
	{
		Safe_Release(Pair.second);
	}
	m_mapTimers.clear();
}
