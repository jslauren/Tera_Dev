#pragma once
#include "Base.h"

_BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	HRESULT Ready_Timer();
	_float Compute_TimeDelta();
private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_CpuTick;
private:
	_float				m_fTimeDelta = 0.f;
public:
	static CTimer*	Create();
	virtual void	Free();
};

_END