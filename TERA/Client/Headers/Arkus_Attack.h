#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Attack : public CArkusState
{
protected:
	explicit CArkus_Attack(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkus_Attack() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Arkus);
	virtual CArkusState*	Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void			Update_State(CArkus& Arkus, const float& fTimeDelta);

private:
	void					MoveArkusPosition(CArkus & Arkus, const _float fArkusSpeed, const _float& fTimeDelta, void * pArg, _int iMoveDir = 0);
	
private:
	_bool					m_IsFlyAtk02StartMove = false;
	
public:
	static CArkus_Attack*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void			Free();

};

_END