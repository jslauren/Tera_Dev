#include "stdafx.h"
#include "..\Headers\Player_Skill_DrawSword.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Skill_DrawSword_End.h"

_USING(Client)

CPlayer_Skill_DrawSword::CPlayer_Skill_DrawSword(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_DrawSword::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::DrawSword);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_DrawSword::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::DrawSword)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.3f))
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(2);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
			return CPlayer_Skill_DrawSword_End::Create(m_pGraphic_Device, Player, &m_iAniState);

	}
	return nullptr;
}

void CPlayer_Skill_DrawSword::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_DrawSword::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(iMoveDir, fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_DrawSword * CPlayer_Skill_DrawSword::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_DrawSword* pInstance = new CPlayer_Skill_DrawSword(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_DrawSword::Free()
{
	CPlayerState::Free();
}
