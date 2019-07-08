#include "..\Headers\Collider.h"
#include "Shader.h"
#include "Transform.h"
#include "Input_Device.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_eType(rhs.m_eType)
	, m_pShader(rhs.m_pShader)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
	, m_pOBBDesc(rhs.m_pOBBDesc)
	, m_bIsRendering(rhs.m_bIsRendering)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pMesh);
	ZeroMemory(&m_ColliderDesc, sizeof(COLLIDERDESC));
}

HRESULT CCollider::Ready_Collider_Prototype(CCollider::TYPE eType)
{
	m_eType = eType;

	HRESULT		hr;

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
	case CCollider::TYPE_OBB:
		hr = Make_Collider_BoundingBox();
		break;
	case CCollider::TYPE_SPHERE:
		hr = Make_Collider_Sphere();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	void*		pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)pVertices, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax)))
		return E_FAIL;

	m_pMesh->UnlockVertexBuffer();

	m_pShader = CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	if (eType == TYPE_OBB ||
		eType == TYPE_AABB)
	{
		m_pOBBDesc = new OBBDESC;
		ZeroMemory(m_pOBBDesc, sizeof(OBBDESC));

		if (FAILED(SetUp_OBBDesc(m_pOBBDesc)))
			return E_FAIL;
	}
	
	return NOERROR;
}

HRESULT CCollider::Ready_Collider(COLLIDERDESC * pArg)
{
	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	return NOERROR;
}

HRESULT CCollider::Update_Collider()
{
	// Feat.슬기
	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_C))
		m_bIsRendering = !m_bIsRendering;

	return NOERROR;
}

HRESULT CCollider::Render_Collider()
{
	if (nullptr == m_pMesh ||
		nullptr == m_pShader)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	Safe_AddRef(pEffect);

	_matrix		matView, matProj;
	
	// 이 콜라이더가 객체에 붙어야하는 상황.
	// 객체의 월드 행렬.
	_matrix		matTransform = *m_ColliderDesc.pTransformMatrix;

	// 객체의 월드 행렬의 스케일정보, 회전정보 삭제.
	if (TYPE_AABB == m_eType)

	{
		// 뼈행렬 * 객체의 월드행렬의 결과행렬의 스케일정보, 회전정보 삭제.
		if (COLLIDERDESC::TYPE_FRAME == m_ColliderDesc.eType)
			matTransform = Reset_AABB_Matrix(*m_ColliderDesc.pFrameMatrix * matTransform);

		// 객체의 월드 행렬의 스케일정보, 회전정보 삭제.
		else if (COLLIDERDESC::TYPE_TRANSFORM == m_ColliderDesc.eType)
			matTransform = Reset_AABB_Matrix(matTransform);
	}

	// 객체의 월드 행렬의 스케일정보 삭제.
	else if (TYPE_OBB == m_eType ||
		TYPE_SPHERE == m_eType)
	{
		// 뼈행렬 * 객체의 월드행렬의 결과행렬의 스케일정보 삭제.
		if (COLLIDERDESC::TYPE_FRAME == m_ColliderDesc.eType)
			matTransform = Reset_OBB_Matrix(*m_ColliderDesc.pFrameMatrix * matTransform);

		// 객체의 월드행렬의 결과행렬의 스케일정보 삭제.
		else
			matTransform = Reset_OBB_Matrix(matTransform);
	}


	//콜라이더의 크기 + 부모기준으로 얼마나 이동.
	_matrix		matColliderTransform;
	D3DXMatrixScaling(&matColliderTransform, m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z);
	memcpy(&matColliderTransform.m[3][0], m_ColliderDesc.vPivot, sizeof(_vec3));
	
	m_matWorld = matColliderTransform * matTransform;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetBool("g_isColl", m_isColl);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if(m_bIsRendering == true)
		m_pMesh->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CCollider::Collision_AABB(const CCollider * pTargetCollider)
{
	m_isColl = false;

	_vec3		vSourMin, vSourMax;
	_vec3		vDestMin, vDestMax;

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &m_matWorld);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &m_matWorld);

	D3DXVec3TransformCoord(&vDestMin, pTargetCollider->Get_Min(), pTargetCollider->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vDestMax, pTargetCollider->Get_Max(), pTargetCollider->Get_WorldMatrix());

	// x축 선상에서 겹쳤니
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	// y축 선상에서 겹쳤니
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	// z축 선상에서 겹쳤니
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	m_isColl = true;

	return _bool(true);
}

_bool CCollider::Collision_OBB(const CCollider * pTargetCollider)
{
	if (nullptr == m_pOBBDesc ||
		nullptr == pTargetCollider->m_pOBBDesc)
		return false;

	m_isColl = false;

	OBBDESC		OBB[2];

	OBB[0] = *m_pOBBDesc;
	OBB[1] = *pTargetCollider->m_pOBBDesc;

	Transform_OBBDesc(&OBB[0], &m_matWorld);
	Transform_OBBDesc(&OBB[1], &pTargetCollider->m_matWorld);


	_float	vDistance[3];

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			_vec3		vCenter;
			vCenter = OBB[1].vCenter - OBB[0].vCenter;

			vDistance[0] = fabs(D3DXVec3Dot(&vCenter, &OBB[i].vAlignAxis[j]));

			vDistance[1] = fabs(D3DXVec3Dot(&OBB[0].vProjAxis[0], &OBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB[0].vProjAxis[1], &OBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB[0].vProjAxis[2], &OBB[i].vAlignAxis[j]));

			vDistance[2] = fabs(D3DXVec3Dot(&OBB[1].vProjAxis[0], &OBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB[1].vProjAxis[1], &OBB[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB[1].vProjAxis[2], &OBB[i].vAlignAxis[j]));

			if (vDistance[0] > vDistance[1] + vDistance[2])
				return false;
		}
	}

	_vec3		vAlignAxis[9];

	D3DXVec3Cross(&vAlignAxis[0], &OBB[0].vAlignAxis[0], &OBB[1].vAlignAxis[0]);
	D3DXVec3Cross(&vAlignAxis[1], &OBB[0].vAlignAxis[0], &OBB[1].vAlignAxis[1]);
	D3DXVec3Cross(&vAlignAxis[2], &OBB[0].vAlignAxis[0], &OBB[1].vAlignAxis[2]);

	D3DXVec3Cross(&vAlignAxis[3], &OBB[0].vAlignAxis[1], &OBB[1].vAlignAxis[0]);
	D3DXVec3Cross(&vAlignAxis[4], &OBB[0].vAlignAxis[1], &OBB[1].vAlignAxis[1]);
	D3DXVec3Cross(&vAlignAxis[5], &OBB[0].vAlignAxis[1], &OBB[1].vAlignAxis[2]);

	D3DXVec3Cross(&vAlignAxis[6], &OBB[0].vAlignAxis[2], &OBB[1].vAlignAxis[0]);
	D3DXVec3Cross(&vAlignAxis[7], &OBB[0].vAlignAxis[2], &OBB[1].vAlignAxis[1]);
	D3DXVec3Cross(&vAlignAxis[8], &OBB[0].vAlignAxis[2], &OBB[1].vAlignAxis[2]);

	for (size_t i = 0; i < 9; ++i)
	{
		_float	vDistance[3];

		_vec3		vCenter;
		vCenter = OBB[1].vCenter - OBB[0].vCenter;

		vDistance[0] = fabs(D3DXVec3Dot(&vCenter, &vAlignAxis[i]));

		vDistance[1] = fabs(D3DXVec3Dot(&OBB[0].vProjAxis[0], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&OBB[0].vProjAxis[1], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&OBB[0].vProjAxis[2], &vAlignAxis[i]));

		vDistance[2] = fabs(D3DXVec3Dot(&OBB[1].vProjAxis[0], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&OBB[1].vProjAxis[1], &vAlignAxis[i])) +
			fabs(D3DXVec3Dot(&OBB[1].vProjAxis[2], &vAlignAxis[i]));

		if (vDistance[0] > vDistance[1] + vDistance[2])
			return false;
	}
	
	m_isColl = true;

	return _bool(true);
}

_bool CCollider::Collision_Sphere(const CCollider * pTargetCollider)
{
	m_isColl = false;

	_float	fDistance = 0.f;
	_vec3	vSourPos, vDestPos;

	memcpy(&vSourPos, &m_matWorld.m[3][0], sizeof(_vec3));
	memcpy(&vDestPos, &pTargetCollider->m_matWorld.m[3][0], sizeof(_vec3));

	m_vLookDistance = vSourPos - vDestPos;

	// 데미지 폰트 포지션용 구문.
	m_vCollisionCheckPosition = (vSourPos + vDestPos) / 2;

	fDistance = D3DXVec3Length(&(vSourPos - vDestPos));

	m_fRadius = (m_ColliderDesc.vScale.x + pTargetCollider->m_ColliderDesc.vScale.x) / 2;

	if (m_fRadius < fDistance)
		return false;
	else
	{
		m_isColl = true;
		return true;
	}
}

HRESULT CCollider::Make_Collider_BoundingBox()
{
	if (FAILED(D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::SetUp_OBBDesc(OBBDESC* pOBBDesc)
{
	pOBBDesc->vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pOBBDesc->vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pOBBDesc->vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pOBBDesc->vPoint[3] = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);

	pOBBDesc->vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pOBBDesc->vPoint[5] = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pOBBDesc->vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pOBBDesc->vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);

	pOBBDesc->vCenter = (pOBBDesc->vPoint[0] + pOBBDesc->vPoint[6]) * 0.5f;

	pOBBDesc->vAlignAxis[0] = _vec3(pOBBDesc->vPoint[2] - pOBBDesc->vPoint[3]);
	pOBBDesc->vAlignAxis[1] = _vec3(pOBBDesc->vPoint[0] - pOBBDesc->vPoint[3]);
	pOBBDesc->vAlignAxis[2] = _vec3(pOBBDesc->vPoint[7] - pOBBDesc->vPoint[3]);

	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&pOBBDesc->vAlignAxis[i], &pOBBDesc->vAlignAxis[i]);
	}
	
	pOBBDesc->vProjAxis[0] = _vec3((pOBBDesc->vPoint[5] + pOBBDesc->vPoint[2]) * 0.5f - pOBBDesc->vCenter);
	pOBBDesc->vProjAxis[1] = _vec3((pOBBDesc->vPoint[5] + pOBBDesc->vPoint[0]) * 0.5f - pOBBDesc->vCenter);
	pOBBDesc->vProjAxis[2] = _vec3((pOBBDesc->vPoint[5] + pOBBDesc->vPoint[7]) * 0.5f - pOBBDesc->vCenter);
	
	return NOERROR;
}

HRESULT CCollider::Transform_OBBDesc(OBBDESC * pOBBDesc, const _matrix * pTransformMatrix)
{
	D3DXVec3TransformCoord(&pOBBDesc->vCenter, &pOBBDesc->vCenter, pTransformMatrix);

	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(&pOBBDesc->vAlignAxis[i], &pOBBDesc->vAlignAxis[i], pTransformMatrix);
		D3DXVec3Normalize(&pOBBDesc->vAlignAxis[i], &pOBBDesc->vAlignAxis[i]);
		D3DXVec3TransformNormal(&pOBBDesc->vProjAxis[i], &pOBBDesc->vProjAxis[i], pTransformMatrix);
	}

	return NOERROR;
}

_matrix CCollider::Reset_AABB_Matrix(const _matrix & matTransform)
{
	_matrix		matTmp;
	D3DXMatrixIdentity(&matTmp);

	memcpy(&(matTmp.m[3][0]), &matTransform.m[3][0], sizeof(_vec3));

	return matTmp;
}

_matrix CCollider::Reset_OBB_Matrix(const _matrix & matTransform)
{
	_matrix		matTmp;
	*D3DXMatrixIdentity(&matTmp) = matTransform;

	D3DXVec3Normalize((_vec3*)&matTmp.m[0][0], (_vec3*)&matTmp.m[0][0]);
	D3DXVec3Normalize((_vec3*)&matTmp.m[1][0], (_vec3*)&matTmp.m[1][0]);
	D3DXVec3Normalize((_vec3*)&matTmp.m[2][0], (_vec3*)&matTmp.m[2][0]);

	return matTmp;
}

HRESULT CCollider::Make_Collider_Sphere()
{
	if (FAILED(D3DXCreateSphere(m_pGraphic_Device, 0.5f, 20, 20, &m_pMesh, nullptr)))
		return E_FAIL;

	return NOERROR;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CCollider::TYPE eType)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Collider_Prototype(eType)))
	{
		_MSGBOX("CCollider Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Ready_Collider((COLLIDERDESC*)pArg)))
	{
		_MSGBOX("CCollider Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	if (false == m_isClone)
		Safe_Delete(m_pOBBDesc);

	Safe_Release(m_pShader);
	Safe_Release(m_pMesh);

	CComponent::Free();
}
