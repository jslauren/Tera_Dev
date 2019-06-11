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

_double CAnimationCtrl::Get_CurretTrackPos()
{
	D3DXTRACK_DESC	TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(D3DXTRACK_DESC));

	// 트랙의 현재 포지션 값.
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	return _double(TrackDesc.Position);
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl()
{
	return NOERROR;
}

HRESULT CAnimationCtrl::SetUp_AnimationSet(_uint iIndex, const _float fAniSpeed)
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

	m_fAniSpeed = fAniSpeed;

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
	m_pAniCtrl->AdvanceTime((fTimeDelta * m_fAniSpeed), nullptr);

	m_TimeAcc += fTimeDelta;

	return NOERROR;
}

_bool CAnimationCtrl::IsAnimationEnded(_double fCtrlEndTime)
{
	// 트랙의 최대 포지션 값.
	double Period = m_pAnimationSet->GetPeriod();

	D3DXTRACK_DESC	TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(D3DXTRACK_DESC));

	// 트랙의 현재 포지션 값.
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	// 애니메이션이 끝났다..!
	// fCtrlEndTime는 원래 애니메이션의 End Time을 조절하기 위한 변수로서,
	// 값을 받아와 Period에서 빼주게 되면,
	// 기존에 끝나는 시간보다 일찍 애니메이션을 종료시킬 수 있어서,
	// 약간의 튀는 애니메이션 보간이 가능하다. (점프같이 3단계로 나뉘어져 있는 애니일때 필요)
	if (Period * fCtrlEndTime <= TrackDesc.Position)
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
