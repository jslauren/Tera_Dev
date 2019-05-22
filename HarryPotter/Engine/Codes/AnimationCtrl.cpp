#include "..\Headers\AnimationCtrl.h"

CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
{
	m_pAniCtrl->AddRef();
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl()
{
	return NOERROR;
}

HRESULT CAnimationCtrl::SetUp_AnimationSet(_uint iIndex)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	if (m_iOldIndex == iIndex)
		return NOERROR;

	LPD3DXANIMATIONSET		pAnimationSet = nullptr;

	if (FAILED(m_pAniCtrl->GetAnimationSet(iIndex, &pAnimationSet)))
		return E_FAIL;

	if (FAILED(m_pAniCtrl->SetTrackAnimationSet(0, pAnimationSet)))
		return E_FAIL;

	m_pAniCtrl->SetTrackPosition(0, 0.0);
	m_pAniCtrl->ResetTime();

	m_iOldIndex = iIndex;

	return NOERROR;
}

HRESULT CAnimationCtrl::SetUp_AnimationSet(const char * pName)
{
	/*if (FAILED(m_pAniCtrl->GetAnimationSetByName(iIndex, &pAnimationSet)))
	return E_FAIL;*/

	return NOERROR;
}

HRESULT CAnimationCtrl::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);

	return NOERROR;
}

CAnimationCtrl * CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl* pInstance = new CAnimationCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		_MSGBOX("CAnimationCtrl Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimationCtrl::Free()
{
	Safe_Release(m_pAniCtrl);
}
