#include "stdafx.h"
#include "..\Headers\QMark.h"
#include "Management.h"
#include "Input_Device.h"

#define QMARK_SCALING	0.7f

_USING(Client)

CQMark::CQMark(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CQMark::CQMark(const CQMark & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQMark::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CQMark::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(QMARK_SCALING, QMARK_SCALING, QMARK_SCALING);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(364.f, 30.f, 364.f));

	return NOERROR;
}

_int CQMark::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CQMark::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

HRESULT CQMark::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshQMarkCom[QMARK_START] ||
		nullptr == m_pMeshQMarkCom[QMARK_ONGOING] ||
		nullptr == m_pMeshQMarkCom[QMARK_REWARD])
		return E_FAIL;

	m_pMeshQMarkCom[m_eCurrentMark]->Play_Animation(m_fTimeDelta);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	// 잘 바뀌는거 확인.
	//if (CInput_Device::GetInstance()->GetDIKeyState(DIK_F) & 0x80)
	//	m_eCurrentMark = QMARK_ONGOING;

	for (size_t i = 0; i < 1; ++i)
	{
		if (FAILED(m_pMeshQMarkCom[m_eCurrentMark]->Update_SkinnedMesh(i)))
			break;

		for (size_t j = 0; j < m_pMeshQMarkCom[m_eCurrentMark]->Get_NumSubSet(i); ++j)
		{
			if (FAILED(m_pMeshQMarkCom[m_eCurrentMark]->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
				return E_FAIL;

			pEffect->CommitChanges();

			pEffect->BeginPass(0);

			m_pMeshQMarkCom[m_eCurrentMark]->Render_Mesh(i, j);

			pEffect->EndPass();
		}
	}

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CQMark::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// 앞에는 컴포넌트 추가를 위한 태그, 뒤에는 추가한 컴포넌트를 찾을때 필요한 태그.
	// 그러므로 이런 상황에서는 다르게 줘야한다.
	// For.Component_Mesh_QMark_Start
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_QMark_Start", L"Com_Mesh_QMark_Start", (CComponent**)&m_pMeshQMarkCom[0])))
		return E_FAIL;

	// For.Component_Mesh_QMark_Ongoing
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_QMark_Ongoing", L"Com_Mesh_QMark_Ongoing", (CComponent**)&m_pMeshQMarkCom[1])))
		return E_FAIL;

	// For.Component_Mesh_QMark_Reward
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_QMark_Reward", L"Com_Mesh_QMark_Reward", (CComponent**)&m_pMeshQMarkCom[2])))
		return E_FAIL;

	return NOERROR;
}

HRESULT CQMark::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	Safe_Release(pEffect);

	return NOERROR;
}

CQMark * CQMark::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQMark* pInstance = new CQMark(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CQMark Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CQMark::Clone(void * pArg)
{
	CQMark* pInstance = new CQMark(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CQMark Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQMark::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	for (size_t i = 0; i < QMARK_END; ++i)
		Safe_Release(m_pMeshQMarkCom[i]);

	CGameObject::Free();
}
