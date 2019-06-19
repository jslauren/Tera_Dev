#pragma once
#include "Defines.h"
#include "Camera.h"

_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{
private:
	explicit CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:	// Getter
	CTransform*		Get_TransformCom() { return m_pTransformCom; }
	_bool			Get_PlayDynamicCamInfo() { return m_bIsDynamicCamOnAir; }

public:	// Setter
	void			Set_TurnOnDynamicCam(_bool bButton) { m_bIsDynamicCamOnAir = bButton; }
	void			Set_CurrentScene(SCENEID eCurrentScene) { m_eCurrentScene = eCurrentScene; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void			DragonTrialCutSceneEvent();

private:
	CTransform*		m_pTransformCom = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ViewMatrix();
	HRESULT			SetUp_ProjectionMatrix();
	
	void			TracingTarget();
	void			KeyInput();
	void			CutSceneEvent();

private:
	_float			m_fTimeDelta = 0.f;
	_float			m_fCameraDistance = 200.f;
	_float			m_fCameraHeight = 200.f;
	_bool			m_bIsDynamicCamOnAir = false;

	SCENEID			m_eCurrentScene = SCENE_STAGE;
	_bool			m_bIsCutSceneEventEnded = false;
	_bool			m_bIsAnimationEneded = false;

public:
	static CCamera_Dynamic*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END