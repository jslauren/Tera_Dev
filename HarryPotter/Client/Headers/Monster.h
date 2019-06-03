#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Dynamic;	// 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer;		//백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
class CCollider;
_END

_BEGIN(Client)

class CMonster final : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;
public:

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
private:
	_float			m_fFrame = 0.f;
	_float			m_fTimeDelta = 0.0f;
private:
	HRESULT Add_Component();
	HRESULT SetUp_HeightOnTerrain();
	HRESULT SetUp_BillBoard();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END