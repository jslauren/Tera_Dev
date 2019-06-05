#pragma once
#include "GameObject.h"
#include "Player.h"

_BEGIN(Client)

//class CPlayer;
class CPlayerState final : public CGameObject
{
public:
	//static IdleState Idle;
private:
	virtual ~CPlayerState() = default;

//public:
//	virtual void	HandleInput(CPlayer& rPlayer, P_STATE eInput) {}
//	virtual void	StateUpdate(CPlayer& Player) {}

};

_END