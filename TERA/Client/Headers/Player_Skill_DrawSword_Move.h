#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer_Skill_DrawSword_Move : public CPlayerState
{
public:
	explicit CPlayer_Skill_DrawSword_Move(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer_Skill_DrawSword_Move() = default;

public:
	virtual HRESULT							Initialize_State(CPlayer& Player);
	virtual CPlayerState*					Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void							Update_State(CPlayer& Player, const float& fTimeDelta);

private:
	void									MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float& fTimeDelta, void * pArg, _int iMoveDir);

public:
	static CPlayer_Skill_DrawSword_Move*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void							Free();

};

_END