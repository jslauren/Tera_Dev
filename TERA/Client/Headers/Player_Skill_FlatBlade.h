#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class Player_Skill_FlatBlade : public CPlayerState
{
protected:
	explicit Player_Skill_FlatBlade(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~Player_Skill_FlatBlade() = default;

public:
	virtual HRESULT						Initialize_State(CPlayer& Player);
	virtual CPlayerState*				Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void						Update_State(CPlayer& Player, const float& fTimeDelta);

private:
	void								MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float& fTimeDelta, void * pArg, _int iMoveDir);
	void								SoundPlay(CPlayer& Player);

public:
	static Player_Skill_FlatBlade*		Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void						Free();
	
};

_END