#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Death : public CArkusState
{
protected:
	explicit CArkus_Death(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkus_Death() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Arkus);
	virtual CArkusState*	Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void			Update_State(CArkus& Arkus, const float& fTimeDelta);

public:
	static CArkus_Death*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void			Free();

};

_END