#include "..\Headers\AnimationCtrl.h"

#define	TRANSITION_TIME 0.2

CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
{
	m_pAniCtrl->AddRef();
}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl & rhs)
{
	if (FAILED(rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAniCtrl->GetMaxNumAnimationSets(), rhs.m_pAniCtrl->GetMaxNumTracks(), rhs.m_pAniCtrl->GetMaxNumEvents(), &m_pAniCtrl)))
		return;
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

	//LPD3DXANIMATIONSET		pAnimationSet = nullptr;

	if (FAILED(m_pAniCtrl->GetAnimationSet(iIndex, &m_pAnimationSet)))
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	if (FAILED(m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, m_pAnimationSet)))
		return E_FAIL;

	//Safe_Release(m_pAnimationSet);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 기존 재생되던 트랙에 대한 설정.
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + TRANSITION_TIME);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 0.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	// 새롭게 재생되어야할 트랙에 대한 설정.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_TimeAcc = 0.0;

	m_pAniCtrl->ResetTime();

	m_iOldIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;

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
	m_pAniCtrl->AdvanceTime((fTimeDelta), nullptr);

	m_TimeAcc += fTimeDelta;

	return NOERROR;
}

_bool CAnimationCtrl::IsAnimationEnded()
{
	// 트랙의 최대 포지션 값.
	double Period = m_pAnimationSet->GetPeriod();
	D3DXTRACK_DESC	TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(D3DXTRACK_DESC));

	// 트랙의 현재 포지션 값.
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	// 애니메이션이 끝났다..!
	if (Period <= TrackDesc.Position)
		return TRUE;

	return _bool(FALSE);
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

CAnimationCtrl * CAnimationCtrl::Create(const CAnimationCtrl & rhs)
{
	CAnimationCtrl* pInstance = new CAnimationCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		_MSGBOX("CAnimationCtrl Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimationCtrl::Free()
{
	Safe_Release(m_pAnimationSet);
	Safe_Release(m_pAniCtrl);
}
