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

public:	// Getter
	CTransform*		Get_TransformCom() { return m_pTransformCom; }
	_bool			Get_PlayStaticCamInfo() { return m_bIsStaticCamOnAir; }
	_float			Get_CemeraDistance() { return m_fCameraDistance; }
	_float			Get_CameraHeightValue() { return m_fCameraHeightValue; }
	_bool			Get_CameraModInfo() { return m_bCameraModOn; }
	_bool			Get_CameraBackTimeInfo() { return m_bCameraBackTime; }

public:	// Setter
	void			Set_TurnOnStaticCam(_bool bButton) { m_bIsStaticCamOnAir = bButton; }
	void			Set_CameraDistance(_float fDistance) { m_fCameraDistance = fDistance; }
	void			Set_CameraHeightValue(_float fHeight) { m_fCameraHeightValue = fHeight; }
	void			Set_CameraModInfo(_bool bButton) { m_bCameraModOn = bButton; }
	void			Set_CameraBackTimeInfo(_bool bButton) { m_bCameraBackTime = bButton; }

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
	_float			m_fCameraDistance = 30.f;
	_float			m_fCameraHeightValue = 10.f;
	_float			m_fDotValuePri = 0.f;
	_bool			m_bIsStaticCamOnAir = true;
	_bool			m_bCameraModOn = true;
	_bool			m_bCameraBackTime = false;

public:
	static CCamera_Static*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};	 

_END