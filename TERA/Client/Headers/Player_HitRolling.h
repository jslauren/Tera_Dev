#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer_HitRolling : public CPlayerState
{
public:
	explicit CPlayer_HitRolling(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer_HitRolling() = default;

public:
	virtual HRESULT				Initialize_State(CPlayer& Player);
	virtual CPlayerState*		Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void				Update_State(CPlayer& Player, const float& fTimeDelta);

public:
	static CPlayer_HitRolling*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void				Free();

};

_END