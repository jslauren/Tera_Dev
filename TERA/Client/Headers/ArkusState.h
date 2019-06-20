#pragma once
#include "Player.h"
#include "Base.h"

_BEGIN(Client)

class CArkus;
class CArkusState : public CBase
{
protected:
	explicit CArkusState(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkusState() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Player);
	virtual CArkusState*	Input_State(CArkus& Player, const float& fTimeDelta, BYTE KeyID, void* pAgr = nullptr);
	virtual void			Update_State(CArkus& Player, const float& fTimeDelta);
	virtual HRESULT			Enter_State(CArkus& Player);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

protected:
	_int					m_iAniState = 1;

public:
	virtual void			Free();

};

_END