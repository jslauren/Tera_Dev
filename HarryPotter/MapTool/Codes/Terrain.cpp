#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "EventManager.h"
#include "Layer.h"
#include "Light_Manager.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pGraphic_Device(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	, m_pGraphic_Device(rhs.m_pGraphic_Device)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CTerrain::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CTerrain::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));

	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	CGameObject::Set_Material(m_MtrlInfo);

	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CTerrain::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (SetUp_ConstantTable(pEffect))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(m_iPassNum);

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

void CTerrain::SetFillMode(_bool bIsWireFrame)
{
	if (true == bIsWireFrame)
		m_iPassNum = 1;
	else
		m_iPassNum = 0;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_Terrain", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Terrain", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));
	
	pEffect->SetVector("g_fDetail", &m_vDetail);

	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	const D3DLIGHT9* pLightInfo = pLight_Manager->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	Safe_Release(pLight_Manager);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 0);

	_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

//HRESULT CTerrain::SetUp_RenderState()
//{
//	if(CEventManagerTool::GetInstance()->m_bIsWireFrame == true)
//		CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//
//	return NOERROR;
//}
//
//HRESULT CTerrain::Release_RenderState()
//{
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//
//	CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//	return NOERROR;
//}

HRESULT CTerrain::Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail)
{
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_vDetail.x = 1;
	m_vDetail.y = 1;
	m_vDetail.z = 1;
	m_vDetail.w = 1;

	m_vDetail *= _fDetail;

	pEffect->SetVector("g_fDetail", &m_vDetail);

	Safe_Release(pEffect);

	if (FAILED(m_pBufferCom->Reset_Terrain(_iNumVtxX, _iNumVtxZ, _fInterval/*, _fDetail*/)))
		return E_FAIL;

	return NOERROR;
	
}

HRESULT CTerrain::Reset_Texture(_tchar* pFilePath)
{
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->Reset_Texture(CTexture_Tool::TYPE_GENERAL, pFilePath, 1);
	
	return NOERROR;
}

// 원본객체를 생성한다.
CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
