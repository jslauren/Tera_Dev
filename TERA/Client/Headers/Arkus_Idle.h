#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Idle : public CArkusState
{
protected:
	explicit CArkus_Idle(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkus_Idle() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Arkus);
	virtual CArkusState*	Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pAgr);
	virtual void			Update_State(CArkus& Arkus, const float& fTimeDelta);

private:
	_bool					EnemyPositionCheck(CArkus& Arkus);
	
private:
	_bool					m_bIsTurnRight = false;

public:
	static CArkus_Idle*		Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void			Free();
};

_END