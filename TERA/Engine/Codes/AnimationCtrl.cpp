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

	// Ʈ���� ���� ������ ��.
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

	// ���� ����Ǵ� Ʈ���� ���� ����.
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + TRANSITION_TIME);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 0.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.f, m_TimeAcc, TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	// ���Ӱ� ����Ǿ���� Ʈ���� ���� ����.
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
	// Ʈ���� �ִ� ������ ��.
	double Period = m_pAnimationSet->GetPeriod();

	D3DXTRACK_DESC	TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(D3DXTRACK_DESC));

	// Ʈ���� ���� ������ ��.
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	// �ִϸ��̼��� ������..!
	// fCtrlEndTime�� ���� �ִϸ��̼��� End Time�� �����ϱ� ���� �����μ�,
	// ���� �޾ƿ� Period���� ���ְ� �Ǹ�,
	// ������ ������ �ð����� ���� �ִϸ��̼��� �����ų �� �־,
	// �ణ�� Ƣ�� �ִϸ��̼� ������ �����ϴ�. (�������� 3�ܰ�� �������� �ִ� �ִ��϶� �ʿ�)
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
