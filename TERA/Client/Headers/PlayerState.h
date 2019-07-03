#pragma once
#include "Player.h"
#include "Arkus.h"
#include "Base.h"

_BEGIN(Client)

class CPlayer;
class CPlayerState : public CBase
{
protected:
	explicit CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayerState() = default;

public:
	virtual HRESULT			Initialize_State(CPlayer& Player);
	virtual CPlayerState*	Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pAgr = nullptr);
	virtual void			Update_State(CPlayer& Player, const float& fTimeDelta);

	// Initialize_State�� State�� ���� ������ ���� �����ϰ�,
	// Enter_State�� �� ���ķ� �ش� State�� �ٲ� ������ �ѹ��� ���ͼ� �����ϴ� ������ ���ش�.
	virtual HRESULT			Enter_State(CPlayer& Player);

protected:
	void					AttackAvailableCheck(CArkus* pArkus, CPlayer* Player);
	void					AttackEvent(CArkus* pArkus, CPlayer* Player, _uint iAvailableHitNumber);
	void					AttackEventFree(CPlayer* Player, CPlayer::PLAYER_ANI ePlayerAni = CPlayer::PLAYER_ANI::END);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

protected:
	_int					m_iAniState = 1;
	_bool					m_bIsDamageAvailable = false;

	_uint					m_iAvailableHitNumber = 1;
	_uint					m_iHitCount = 0;

public:
	virtual void			Free();

};

_END