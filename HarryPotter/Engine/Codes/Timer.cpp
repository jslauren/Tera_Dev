#include "..\Headers\Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

_float CTimer::Compute_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);

	// 1ÃÊ°¡ Áö³µ¾î.
	if (m_FrameTime.QuadPart - m_LastTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;
	m_FixTime = m_FrameTime;


	return _float(m_fTimeDelta);
}

CTimer * CTimer::Create()
{
	CTimer* pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Timer()))
	{
		_MSGBOX("CTimer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{
}
