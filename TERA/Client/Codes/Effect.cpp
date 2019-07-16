#include "stdafx.h"
#include "..\Headers\Effect.h"
#include "Target_Manager.h"

_USING(Client)

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fFrame(0.f)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
	, m_fFrame(rhs.m_fFrame)
{
}

HRESULT CEffect::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEffect::Ready_GameObject(void * pArg)
{
	return NOERROR;
}

_int CEffect::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CEffect::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CEffect::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CEffect::OnEvent(const _tchar * _szEventTag, void * _pMsg)
{
	return NOERROR;
}

HRESULT CEffect::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Shader_Effect", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CEffect::SetUp_BillBoard()
{
	CTransform* pCamTransformCom = (CTransform*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Camera", L"Com_Transform", 1);
 	if (nullptr == pCamTransformCom)
		return E_FAIL;

	Safe_AddRef(pCamTransformCom);

	// 카메라의 월드행렬 == 뷰스페이스 변환행렬 역행렬
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pCamTransformCom->Get_StateInfo(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, pCamTransformCom->Get_StateInfo(CTransform::STATE_LOOK));

	Safe_Release(pCamTransformCom);

	return NOERROR;
}

HRESULT CEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", _uint(m_fFrame));

	if (FAILED(CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return E_FAIL;

	Safe_Release(pEffect);

	return NOERROR;
}

void CEffect::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
