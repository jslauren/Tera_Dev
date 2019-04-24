#pragma once
#include "GameObject.h"

_BEGIN(Engine)

class _DLL_EXPORTS CCamera : public CGameObject
{
public:
	typedef struct tagCameraInfo
	{
		tagCameraInfo()
		{
		}

		tagCameraInfo(const _vec3& _vEye, const _vec3& _vAt, const _vec3& _vAxisY,
			_float _fFovY, _float _fAspect, _float _fNear, _float _fFar)
			: vEye(_vEye), vAt(_vAt), vAxisY(_vAxisY)
			, fFovY(_fFovY), fAspect(_fAspect), fNear(_fNear), fFar(_fFar) {}

		_vec3		vEye; // 위치
		_vec3		vAt; // 바라보는 점. 
		_vec3		vAxisY; // 업

		_float		fFovY;
		_float		fAspect;
		_float		fNear;
		_float		fFar;
	}CAMERAINFO;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
protected:
	CAMERAINFO		m_CameraInfo;
public:
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();
};

_END