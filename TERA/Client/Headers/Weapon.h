#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Static; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
class CCollider;
_END

_BEGIN(Client)

class CWeapon final : public CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:	// Getter
	CTransform*	Get_TransformCom() { return m_pTransformCom; }
	CCollider*	Get_ColliderTop() { return m_pColliderCom01; }

public: // Setter
	HRESULT			Set_BoneMatrix(_int iIndex);

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
	
public:
	_bool			CollisionCheck();

private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Static*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom01 = nullptr;
	CCollider*		m_pColliderCom02 = nullptr;
	CCollider*		m_pColliderCom03 = nullptr;
	CCollider*		m_pColliderCom04 = nullptr;

private:
	_float			m_fFrame = 0.f;
	_float			m_fTimeDelta = 0.0f;
	const _matrix*	m_pBoneMatrix = nullptr;
	const _matrix*	m_pParentMatrix = nullptr;
	_matrix			m_matWorld;
	_bool			m_bCollisionCheck = false;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_HeightOnTerrain();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CWeapon*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END