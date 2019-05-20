#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "EventManager.h"
#include "Layer.h"

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
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->SetUp_OnGraphicDev(0);

	SetUp_RenderState();

	m_pBufferCom->Render_Buffer(m_pTransformCom);

	Release_RenderState();

	return NOERROR;
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

	return NOERROR;
}

HRESULT CTerrain::SetUp_RenderState()
{
	if(CEventManagerTool::GetInstance()->m_bIsWireFrame == true)
		CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return NOERROR;
}

HRESULT CTerrain::Release_RenderState()
{
	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return NOERROR;
}

HRESULT CTerrain::Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail)
{
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	
	if (FAILED(m_pBufferCom->Reset_Terrain(_iNumVtxX, _iNumVtxZ, _fInterval, _fDetail)))
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

void CTerrain::DrawAxis()
{
	//D3DXVECTOR3 p[3];
	//p[0].x = 0; p[0].y = 0; p[0].z = 0.0f;
	//p[1].x = 0.1; p[1].y = 1; p[1].z = 0.0f;
	//p[2].x = 2; p[2].y = 0; p[2].z = 0.0f;

	//ID3DXLine *Line;
	//D3DXCreateLine(m_pGraphic_Device, &Line);
	//Line->SetWidth(2.f);
	//Line->SetAntialias(true);
	//Line->Begin();
	//Line->DrawTransform(p, 3, m_pTransformCom->Get_WorldMatrixPointer(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	////Line->Draw( p, 3, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f) );    // 이런식으로도 그리기 가능
	//Line->End();
	//Line->Release();
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
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
