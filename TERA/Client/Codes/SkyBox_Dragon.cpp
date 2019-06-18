#include "stdafx.h"
#include "..\Headers\SkyBox_Dragon.h"
#include "Object_Manager.h"

_USING(Client)

CSkyBox_Dragon::CSkyBox_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSkyBox_Dragon::CSkyBox_Dragon(const CSkyBox_Dragon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox_Dragon::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSkyBox_Dragon::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CSkyBox_Dragon::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CSkyBox_Dragon::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return -1;

	pObject_Manager->AddRef();

	CTransform*	pCameraTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_DRAGON, L"Layer_Camera", L"Com_Transform", 1);
	if (nullptr == pCameraTransform)
		return -1;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pCameraTransform->Get_StateInfo(CTransform::STATE_POSITION));

	Safe_Release(pObject_Manager);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CSkyBox_Dragon::Render_GameObject()
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
	pEffect->BeginPass(0);

	// ��� = ��� * ���
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CSkyBox_Dragon::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Buffer_CubeBox", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Sky", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSkyBox_Dragon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 2);

	Safe_Release(pEffect);

	return NOERROR;
}

//HRESULT CSkyBox_Dragon::SetUp_RenderState()
//{
//
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//
//	// ť������� ������ �ٱ������� �ٶ󺸰� �ְԲ� �۾�. 
//	// ī�޶�� ť��ȿ� ����. ť�� ������ �޸��� �ٶ󺸴� ȿ��.
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CW);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
//
//	// ��ī�̹ڽ��� �ȼ� ���̸� ���̹��ۿ� �������� �ʴ´�.
//	// ��ī�̹ڽ� ���Ŀ� �׷����� �ȼ��Y���� ��ī�̹ڽ� �ȼ����̿� ���� �� ����. ������ ������ ���ı׷����� �ֵ��� ���� �׸���.
//	CGameObject::Set_RenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	return NOERROR;
//}
//
//HRESULT CSkyBox_Dragon::Release_RenderState()
//{
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	CGameObject::Set_RenderState(D3DRS_ZWRITEENABLE, TRUE);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, TRUE);
//
//	return NOERROR;
//}

// ������ü�� �����Ѵ�.
CSkyBox_Dragon * CSkyBox_Dragon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkyBox_Dragon* pInstance = new CSkyBox_Dragon(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CSkyBox_Dragon Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.����Լ��ȿ� ����. 
// 2.����Լ��� ��ü�κ��� ȣ��(��ü.����Լ�(), ��ü�ּ�->����Լ�())
// 3.����Լ��ȿ� �����ϴ� this�� ����Լ��� ȣ���� �����ϰ��� ��ü�� �ּҸ� �ǹ��Ѵ�.
CGameObject * CSkyBox_Dragon::Clone(void* pArg)
{
	CSkyBox_Dragon* pInstance = new CSkyBox_Dragon(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CSkyBox_Dragon Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox_Dragon::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
