#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Apperance : public CArkusState
{
public:
	CArkus_Apperance(LPDIRECT3DDEVICE9 pGraphic_Device);
	~CArkus_Apperance() = default;

public:
	virtual HRESULT				Initialize_State(CArkus& Arkus);
	virtual CArkusState*		Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pAgr);
	virtual void				Update_State(CArkus& Arkus, const float& fTimeDelta);

public:
	static CArkus_Apperance*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void				Free();

};

_END