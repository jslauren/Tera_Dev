#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Static; // �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer; //��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
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

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public: // Setter
	HRESULT			Set_BoneMatrix(_int iIndex);

private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Static*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
private:
	_float			m_fFrame = 0.f;
	_float			m_fTimeDelta = 0.0f;
	const _matrix*	m_pBoneMatrix = nullptr;
	const _matrix*	m_pParentMatrix = nullptr;
	_matrix			m_matWorld;
private:
	HRESULT			Add_Component();
	HRESULT			SetUp_HeightOnTerrain();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

	void			CollisionCheck();

public:
	static CWeapon*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END