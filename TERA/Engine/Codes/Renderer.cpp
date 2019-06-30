#include "..\Headers\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Buffer_ScreenTex.h"
#include "Input_Device.h"

#include "Shader.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
}

HRESULT CRenderer::Ready_Renderer()
{
	// For.RenderTarget
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9	ViewPort;

	m_pGraphic_Device->GetViewport(&ViewPort);

	// For.Target_Diffuse
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Shade
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.MRT_Deferred(Diffuse + Normal + Depth)
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT_LightAcc(Shade + Specular)
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	// For.Shader_LightAcc
	m_pShader_LightAcc = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	m_pShader_Blend = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Blend.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = CBuffer_ScreenTex::Create(m_pGraphic_Device, 0.f - 0.5f, 0.f - 0.5f, ViewPort.Width, ViewPort.Height);
	if (nullptr == m_pBuffer)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Diffuse", 0.f, 0.f, 200.0f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Normal", 0.f, 200.f, 200.0f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Depth", 0.f, 400.f, 200.0f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Shade", 200.f, 0.f, 200.0f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(L"Target_Specular", 200.f, 200.f, 200.0f, 200.f)))
		return E_FAIL;
#endif

	return NOERROR;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

HRESULT CRenderer::Render_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	// 디퓨즈 노말에 필요한 정보를 그렸다.
	if (FAILED(Render_Deferrerd()))
		return E_FAIL;

	// 셰이드타겟에 노말을 이용해서 명암을 그린다.
	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	// 백버퍼에 출력해야할 픽셀들을 그린다.(디퓨즈타겟 * 셰이드타겟)
	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_TAB))
		m_bIsRendering = !m_bIsRendering;

#ifdef _DEBUG

	if (m_bIsRendering == true)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(L"MRT_Deferred")))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(L"MRT_LightAcc")))
			return E_FAIL;
	}
	
#endif

	return NOERROR;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_NoneAlpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONEALPHA].clear();

	return NOERROR;
}

_bool Compare_Z(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Compare_Z);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_ALPHA].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_UI].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_Deferrerd()
{
	// 디퓨즈타겟 노말타겟을 장치에 셋한다.
	// 디퍼드 멀티렌더타겟을 장치에 셋한다.
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return E_FAIL;

	if (FAILED(Render_NoneAlpha()))
		return E_FAIL;

	// 백버퍼로 복구한다.
	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pLight_Manager ||
		nullptr == m_pShader_LightAcc)
		return E_FAIL;

	// 셰이드 타겟을 장치에 셋한다.
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader_LightAcc->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	m_pLight_Manager->Render_LightAcc(pEffect);

	pEffect->End();

	Safe_Release(pEffect);

	// 백버퍼가 장치의 0번째 소켓에 셋팅되었다.
	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pShader_Blend ||
		nullptr == m_pTarget_Manager)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader_Blend->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture")))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 백버퍼에 픽셀을 그립니다.
	m_pBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	return NOERROR;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		_MSGBOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pShader_LightAcc);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);

	CComponent::Free();
}
