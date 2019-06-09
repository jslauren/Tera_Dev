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

HRESULT CTransform::Set_Scaling(const _float & fX, const _float & fY, const _float & fZ)
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
	_vec3		vRight, vUp, vLook;

	vRight = *D3DXVec3Normalize(&vRight, (_vec3*)&m_matWorld.m[0][0]) * fX;
	memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));

	vUp = *D3DXVec3Normalize(&vUp, (_vec3*)&m_matWorld.m[1][0]) * fY;
	memcpy(&m_matWorld.m[1][0], &vUp, sizeof(_vec3));

	vLook = *D3DXVec3Normalize(&vLook, (_vec3*)&m_matWorld.m[2][0]) * fZ;
	memcpy(&m_matWorld.m[2][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Set_Rotation_YawPitchRoll(_float fRadianY, _float fRadianX, _float fRadianZ)
{
	m_vRotRadValue.x = fRadianX;
	m_vRotRadValue.y = fRadianY;
	m_vRotRadValue.z = fRadianZ;

	_matrix		matRotate;

	D3DXMatrixRotationYawPitchRoll(&matRotate, fRadianY, fRadianX, fRadianZ);

	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
	D3DXVec3TransformNormal(&vRight, &vRight, &matRotate);

	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRotate);

	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotate);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Set_Angle_Axis(_vec3 vState, const _float & fRadian)
{
	// �� �Լ��� ������ ������ �������� ���� ������ fRadian ����ŭ ���� ���·�,
	// �ʱ�ȭ�� ���ְڴ� ��� �Լ��̴�.
	// ���� ���������� �ٲ��� �ʰ� �ѹ��� ������ �ֱ� ������ fTimeDelta���� �ʿ� ����.

	_matrix		matRotate;
	D3DXMatrixRotationAxis(&matRotate, &vState, fRadian);

	// �׷��� �̷��� �ʱⰪ�� 1�� ������ �׵���� ���� ���͸� ���� ���ִµ�,
	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	// �̷��� �ϸ� �� ������ ���� ũ�������� ����� ������ ������,
	// D3DXVec3Length�Լ��� ����Ͽ�, �� ���͸� �ٽ� �ʱ�ȭ? ���ش�.
	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
	D3DXVec3TransformNormal(&vRight, &vRight, &matRotate);

	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRotate);

	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotate);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Move(_int iDirection, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	// ��ġ = ��ġ + ���� �ٶ󺸰� �ִ� ���⺤��(=vLook) (�� ��, ����� ũ�⸦ 1��) * �ӵ�PerSec * fTimeDelta
	// ��������� 4�࿡ ���ŵ� ��ġ������ ��������ش�.(�������ش�)

	_vec3	vPosition, vLook, vRight;

	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));

	if (iDirection == 0)
		vPosition += ((*D3DXVec3Normalize(&vLook, &vLook)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 1)
		vPosition -= ((*D3DXVec3Normalize(&vLook, &vLook)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 2)
		vPosition -= ((*D3DXVec3Normalize(&vRight, &vRight)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 3)
		vPosition += ((*D3DXVec3Normalize(&vRight, &vRight)) * fSpeedPerSec * fTimeDelta);

	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));

	return NOERROR;
}

//HRESULT CTransform::Set_Angle_X(const _float & fRadian)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationX(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Set_Angle_Y(const _float & fRadian)
//{
//	_matrix		matRot;
//
//	D3DXMatrixRotationY(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Set_Angle_Z(const _float & fRadian)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationZ(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Go_Straight(const _float& fSpeed, const _float & fTimeDelta)
//{
//	// ��ġ = ��ġ + ���� �ٶ󺸰� �ִ� ���⺤��(����, ũ�⸦ 1��) * �ӵ�PerSec * fTimeDelta
//	// ��������� 4�࿡ ���ŵ� ��ġ������ ��������ش�.(�������ش�)
//
//
//	_vec3			vPosition, vLook;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//
//	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Back_Straight(const _float & fSpeed, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vLook;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//
//	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Move_Right(const _float & fSpeedPerSec, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vRight;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//
//	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * fSpeedPerSec * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Move_Left(const _float & fSpeedPerSec, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vRight;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//
//	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * fSpeedPerSec * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_X(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationX(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_Y(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationY(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_Z(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationZ(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}

HRESULT CTransform::Rotation_Axis(const _vec3 & vAxis, const _float & fRadianPerSec, const _float & fTimeDelta)
{
	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxis, fRadianPerSec * fTimeDelta);

	_vec3		vRight, vUp, vLook;

	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);

	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);

	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Move_Target(const CTransform * pTransform, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	_vec3		vTargetPos = *pTransform->Get_StateInfo(STATE_POSITION);
	_vec3		vPosition = *Get_StateInfo(STATE_POSITION);
	_vec3		vDir = vTargetPos - vPosition;

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * fSpeedPerSec * fTimeDelta;

	vDir *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_LOOK));

	_vec3	vUp = *Get_StateInfo(CTransform::STATE_UP);

	_vec3	vRight;
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &vUp, &vDir));

	vRight *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_RIGHT));

	Set_StateInfo(STATE_RIGHT, &vRight);

	Set_StateInfo(STATE_LOOK, &vDir);

	Set_StateInfo(STATE_POSITION, &vPosition);

	return NOERROR;
}

HRESULT CTransform::Move_Target(const _vec3 * pTargetPos, const _float & fSpeedPerSec, const _float & fTimeDelta, _bool* pFinish)
{
	_vec3		vTargetPos = *pTargetPos;

	_vec3		vPosition = *Get_StateInfo(STATE_POSITION);
	_vec3		vDir = vTargetPos - vPosition;

	_float		fLength = D3DXVec3Length(&vDir);

	if (0.5f > fLength)
	{
		*pFinish = true;
		return NOERROR;
	}

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * fSpeedPerSec * fTimeDelta;

	vDir *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_LOOK));

	_vec3	vUp = *Get_StateInfo(CTransform::STATE_UP);

	_vec3	vRight;
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &vUp, &vDir));

	vRight *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_RIGHT));

	Set_StateInfo(STATE_RIGHT, &vRight);

	Set_StateInfo(STATE_LOOK, &vDir);

	Set_StateInfo(STATE_POSITION, &vPosition);

	*pFinish = false;

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

CComponent * CTransform::Clone(void* pArg)
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
