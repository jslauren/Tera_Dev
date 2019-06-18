#include "stdafx.h"
#include "..\Headers\Unit.h"

_USING(Client)

CUnit::CUnit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUnit::CUnit(const CUnit & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUnit::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CUnit::Ready_GameObject(void * pArg)
{
	return NOERROR;
}

_int CUnit::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CUnit::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CUnit::Render_GameObject()
{
	return NOERROR;
}

HRESULT CUnit::Add_Component()
{
	return NOERROR;
}

HRESULT CUnit::SetUp_HeightOnTerrain(_uint iIndex)
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", iIndex);
	if (nullptr == pBufferCom)
		return E_FAIL;

	// 플레이어의 Y값과 이 지형의 Y값을 비교해서, 점프를 구현하면 된다.
	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY /*+ 0.5f*/, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CUnit::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}


void CUnit::Free()
{
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransformRotateCom);
	Safe_Release(m_pTransformMoveCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
