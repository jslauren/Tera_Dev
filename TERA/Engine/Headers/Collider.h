#pragma once

// �浹�� ���. 
// �浹�ϱ����� ����(�޽�)�� ������ �ִ� ��ü��.
#include "Component.h"

_BEGIN(Engine)

class CShader;
class CTransform;
class _DLL_EXPORTS CCollider final : public CComponent
{
public:
	// ������ 1, ������ ��ġ�� �ݶ��̴��� �����ϰ�

	// x : ���º�ȯ�� �����Ҷ� �����ҷ����߾� �ٵ� �����ϸ� �浹�Ҷ����� �������Ҷ� ��뤷�ߴ� �ۤ����� �����;��ߴܸ��̾� �׷��� �Ⱦ�.
	// o : �ݶ��̴� ������(��ü�� ������Ʈ�� �߰��ɶ�)�� �� �ݶ��̴��� ��ȯ�Ǿ��ϴ� ��������� ����(�ּ�)�� ������� ����ü.
	typedef struct tagColliderDesc
	{
		enum TARGETTYPE { TYPE_FRAME, TYPE_TRANSFORM };

		tagColliderDesc() { }
		tagColliderDesc(tagColliderDesc::TARGETTYPE eType_In,
			const _matrix* pTransformPointer,
			const _matrix* pFramePointer,
			_vec3 vScale_In, _vec3 vPivot_In)
			: eType(eType_In) , pFrameMatrix(pFramePointer) , pTransformMatrix(pTransformPointer), vScale(vScale_In), vPivot(vPivot_In) { }

		TARGETTYPE		eType;

		_vec3		vScale;
		_vec3		vPivot;

		const _matrix*		pFrameMatrix;
		const _matrix*		pTransformMatrix;

	}COLLIDERDESC;

	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vAlignAxis[3];
		_vec3		vProjAxis[3];

	}OBBDESC;
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	const _vec3* Get_Min() const { return &m_vMin; }
	const _vec3* Get_Max() const { return &m_vMax; }
	const _matrix* Get_WorldMatrix() const { return &m_matWorld; }
public:
	HRESULT Ready_Collider_Prototype(CCollider::TYPE eType);
	HRESULT Ready_Collider(COLLIDERDESC* pArg);
	HRESULT Render_Collider();
public:
	_bool Collision_AABB(const CCollider* pTargetCollider);
	_bool Collision_OBB(const CCollider* pTargetCollider);
private:
	LPD3DXMESH		m_pMesh = nullptr;
	TYPE			m_eType = TYPE_END;
	COLLIDERDESC	m_ColliderDesc;
	CShader*		m_pShader = nullptr;
	_bool			m_isColl = false;
private:
	_vec3			m_vMin, m_vMax;
	_matrix			m_matWorld;
	OBBDESC*		m_pOBBDesc = nullptr;
private: // For.Cube Collider
	HRESULT Make_Collider_BoundingBox();
	HRESULT SetUp_OBBDesc(OBBDESC* pOBBDesc);
	HRESULT Transform_OBBDesc(OBBDESC* pOBBDesc, const _matrix* pTransformMatrix);
	_matrix Reset_AABB_Matrix(const _matrix& matTransform);
	_matrix Reset_OBB_Matrix(const _matrix& matTransform);
private: // For.Sphere Collider
	HRESULT Make_Collider_Sphere();
public:
	static CCollider*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CCollider::TYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

_END