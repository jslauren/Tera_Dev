#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer_AttackCombo : public CPlayerState
{
protected:
	explicit CPlayer_AttackCombo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer_AttackCombo() = default;

public:
	virtual HRESULT				Initialize_State(CPlayer& Player);
	virtual CPlayerState*		Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pAgr);
	virtual void				Update_State(CPlayer& Player, const float& fTimeDelta);

private:
	void						MovePlayerPosition(CPlayer & Player, _float fPointTime, _float fPlayerSpeed, const float& fTimeDelta, void * pArg);
	void						SoundPlay(CPlayer& Player);

public:
	static CPlayer_AttackCombo*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void				Free();

};

_END