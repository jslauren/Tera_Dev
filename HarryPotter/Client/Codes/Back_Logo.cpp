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
	//m_pTransformCom->Set_Angle_Y(D3DXToRadian(45.0f));

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	//if (GetKeyState('W') & 0x8000)
	//{
	//	m_pTransformCom->Go_Straight(5.f, fTimeDelta);
	//}

	//if (GetKeyState('S') & 0x8000)
	//{
	//	m_pTransformCom->Back_Straight(5.f, fTimeDelta);
	//}

	//if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_X(D3DXToRadian(90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_X(D3DXToRadian(-90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_Y(D3DXToRadian(-90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_Y(D3DXToRadian(90.f), fTimeDelta);
	//}

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
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->SetUp_OnGraphicDev(0);

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer(m_pTransformCom);

	return NOERROR;
}

HRESULT CBack_Logo::Add_Component()
{
	// 사용할 컴포넌트 객체들을 이 함수에서 추가해준다.

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
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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
	// *this 는 원본객체에 해당 함.
	// 복사 생성자를 호출하면 부모의 복사 생성자도 호출한다.
	// 장치 객체 때문에 얕은 복사를 하면 문제가 생긴다.
	// (하지만 포인터는 얕은복사해도 상관이 없다고 한다)
	// AddRefCnt 때문에 얕은 복사 하면 안된다.
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

	CGameObject::Free();
}
