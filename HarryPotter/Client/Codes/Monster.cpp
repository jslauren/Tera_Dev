#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Object_Manager.h"

_USING(Client)

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fFrame(0.f)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
	, m_fFrame(rhs.m_fFrame)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CMonster::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CMonster::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(1.f, 1.f, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(rand() % 10 + 5, 0.f, rand() % 10 + 5));

	return NOERROR;
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fFrame += 90.0f * fTimeDelta;

	if (90.0f <= m_fFrame)
		m_fFrame = 0.f;

	return _int();
}

_int CMonster::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	if (FAILED(SetUp_HeightOnTerrain()))
		return -1;

	if (FAILED(SetUp_BillBoard()))
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

HRESULT CMonster::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->SetUp_OnGraphicDev(_uint(m_fFrame));

	SetUp_RenderState();

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer(m_pTransformCom);

	Release_RenderState();

	return NOERROR;
}

HRESULT CMonster::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Effect", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMonster::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMonster::SetUp_BillBoard()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CTransform* pCamTransformCom = (CTransform*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 0);
	if (nullptr == pCamTransformCom)
		return E_FAIL;

	// 카메라의 월드행렬 == 뷰스페이스 변환행렬 역행렬
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT
		, pCamTransformCom->Get_StateInfo(CTransform::STATE_RIGHT));
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_UP
	//	, pCamTransformCom->Get_StateInfo(CTransform::STATE_UP));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK
		, pCamTransformCom->Get_StateInfo(CTransform::STATE_LOOK));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMonster::SetUp_RenderState()
{
	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CW);

	CGameObject::Set_RenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CGameObject::Set_RenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGameObject::Set_RenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*CGameObject::Set_RenderState(D3DRS_ALPHATESTENABLE, TRUE);
	CGameObject::Set_RenderState(D3DRS_ALPHAREF, 0);
	CGameObject::Set_RenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	return NOERROR;
}

HRESULT CMonster::Release_RenderState()
{
	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	CGameObject::Set_RenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//CGameObject::Set_RenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return NOERROR;
}

// 원본객체를 생성한다.
CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CMonster Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
CGameObject * CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CMonster Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
