#include "..\Headers\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
{
}

HRESULT CTransform::SetUp_OnGraphicDev()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// D3DTS_WORLD
	// D3DTS_VIEW
	// D3DTS_PROJECTION

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	return NOERROR;
}

HRESULT CTransform::Scaling(const _float & fX, const _float & fY, const _float & fZ)
{
	// [���ظ� ���� ���� ���] //
	//_vec3	 vRight;

	//// ������� 1���� Right���� ù��° �ּҰ��� &m_matWorld.m[0][0]����,
	//// sizeof(_vec3)) ũ�⸸ŭ��, �׷��ϱ� float 3�� ��ŭ�� ũ�⸦ ī�� �Ѵٴ� �Ŵϱ�,
	//// [0][0], [0][1], [0][2]�� �� ī���Ѵ� �̸��� �ǹ�����.
	//memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
	//
	//vRight = *D3DXVec3Normalize(&vRight, &vRight) * fX;

	//memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));

	// [������� ������ ���] //
	_vec3	vRight, vUp, vLook;

	vRight = *D3DXVec3Normalize(&vRight, (_vec3*)&m_matWorld.m[0][0]) * fX;
	memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));

	vUp = *D3DXVec3Normalize(&vUp, (_vec3*)&m_matWorld.m[1][0]) * fY;
	memcpy(&m_matWorld.m[1][0], &vUp, sizeof(_vec3));

	vLook = *D3DXVec3Normalize(&vLook, (_vec3*)&m_matWorld.m[2][0]) * fZ;
	memcpy(&m_matWorld.m[2][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Transform()))
	{
		_MSGBOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
