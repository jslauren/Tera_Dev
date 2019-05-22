#pragma once
#include "Base.h"

_BEGIN(Engine)

class CAnimationCtrl final : public CBase
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	virtual ~CAnimationCtrl() = default;
public:
	HRESULT Ready_AnimationCtrl();
	HRESULT SetUp_AnimationSet(_uint iIndex);
	HRESULT SetUp_AnimationSet(const char* pName);
	HRESULT Play_Animation(const _float& fTimeDelta);
private:
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl = nullptr;
	_uint		m_iOldIndex = 0;
public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	virtual void Free();
};

_END