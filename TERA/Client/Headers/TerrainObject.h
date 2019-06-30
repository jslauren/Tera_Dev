#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Static; // �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer; //��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
class CFrustum;
_END

_BEGIN(Client)

class CTerrainObject final : public CGameObject
{
private:
	explicit CTerrainObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrainObject(const CTerrainObject& rhs);
	virtual ~CTerrainObject() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Static*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CFrustum*		m_pFrustumCom = nullptr;

private:
	_float			m_fTimeDelta = 0.0f;
	_float			m_fCulling = 95.f;

private:
	HRESULT			Add_Component(void* pArg);
//	HRESULT			SetUp_HeightOnTerrain();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CTerrainObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END