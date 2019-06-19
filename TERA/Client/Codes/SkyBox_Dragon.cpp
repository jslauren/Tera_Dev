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

	Set_CameraChoice();

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

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

void CSkyBox_Dragon::Set_CameraChoice()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return;

	pObject_Manager->AddRef();

	CTransform*	pCameraTransform = nullptr;

	// 이 Drangon's Trial Scene은 컷신 이벤트가 있기때문에,
	// 다이나믹 카메라를 사용하므로, Default 값으로 다이나믹 카메라를 설정 하였으며,
	// 다이나믹 카메라의 포지션 값을 스카이박스에 넣어준다.

	// 이 후, 컷신 이벤트가 종료되면 다시 스태틱 카메라로 돌아가기 때문에,
	// 외부에서 컷신 이벤트가 종료되었을 때, 이 함수를 불러 스태틱 카메라의 정보를 셋팅 해준다.
	if (m_iCameraChoiceNum == 0)
		pCameraTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_DRAGON, L"Layer_Camera", L"Com_Transform", 0);
	
	else if(m_iCameraChoiceNum == 1)
		pCameraTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_DRAGON, L"Layer_Camera", L"Com_Transform", 1);

	if (nullptr == pCameraTransform)
		return;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pCameraTransform->Get_StateInfo(CTransform::STATE_POSITION));

	Safe_Release(pObject_Manager);
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
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
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
