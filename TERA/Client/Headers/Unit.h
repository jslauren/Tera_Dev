#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Dynamic;
class CRenderer; 
class CTexture;
class CCollider;
class CFrustum;
class CShader;
class CNavigation;
_END

_BEGIN(Client)

class CUnit : public CGameObject
{
protected:
	explicit CUnit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUnit(const CUnit& rhs);
	virtual ~CUnit() = default;

public: // Getter
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CTransform*		Get_TransformRotation() { return m_pTransformRotateCom; }
	CTransform*		Get_TransformMove() { return m_pTransformMoveCom; }
	CMesh_Dynamic*	Get_Mesh() { return m_pMeshCom; }
	CNavigation*	Get_NaviMesh() { return m_pNavigationCom; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
	virtual HRESULT	OnEvent(const _tchar * _szEventTag, void * _pMsg);

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_HeightOnTerrain(_uint iIndex);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pTransformRotateCom = nullptr;
	CTransform*		m_pTransformMoveCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CFrustum*		m_pFrustumCom = nullptr;
	CNavigation*	m_pNavigationCom = nullptr;

//	CUnit*			m_pTargetUnit = nullptr;

	_float			m_fFrame = 0.f;
	_float			m_fAniSpeed = 1.f;
	_float			m_fTimeDelta = 1.f;
	_bool			m_bIsRun = false;

	_float			m_fHP = 100.f;
	_float			m_fMP = 100.f;

public:
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();

};

_END