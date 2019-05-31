#pragma once
#include "Defines.h"
#include "Camera.h"

_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)

class CCamera_Static final : public CCamera
{
private:
	explicit CCamera_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Static(const CCamera_Static& rhs);
	virtual ~CCamera_Static() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*		m_pTransformCom = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;
private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ViewMatrix();
	HRESULT			SetUp_ProjectionMatrix();
	void			ChangeView();	// Using_Mouse
	void			TracingPlayer();
private:
	_float			m_fTimeDelta = 0.f;
public:
	static CCamera_Static*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};	 

_END