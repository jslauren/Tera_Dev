#include "stdafx.h"
#include "..\Headers\Back_Logo.h"

_USING(Client)

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBack_Logo::CBack_Logo(const CBack_Logo & rhs)
	: CGameObject(rhs)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CBack_Logo::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(2.f, 2.f, 2.f);

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix			matTmp;
	D3DXMatrixIdentity(&matTmp);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CBack_Logo::Add_Component()
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

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

// 원본객체를 생성한다.
CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
CGameObject * CBack_Logo::Clone(void* pArg)
{
	CBack_Logo* pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Logo::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
