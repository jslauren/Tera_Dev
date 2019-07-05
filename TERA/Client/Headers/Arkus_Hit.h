#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Hit : public CArkusState
{
protected:
	explicit CArkus_Hit(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkus_Hit() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Arkus);
	virtual CArkusState*	Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void			Update_State(CArkus& Arkus, const float& fTimeDelta);

private:
	_uint					m_iRandomState = 0;

public:
	static CArkus_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void			Free();

};

_END