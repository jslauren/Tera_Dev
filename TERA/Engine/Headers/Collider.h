#pragma once

// 충돌의 기능. 
// 충돌하기위한 정점(메시)을 가지고 있는 객체다.
#include "Component.h"

_BEGIN(Engine)

class CShader;
class CTransform;
class _DLL_EXPORTS CCollider final : public CComponent
{
public:
	// 사이즈 1, 원점에 위치한 콜라이더를 생성하고

	// x : 상태변환을 렌더할때 적용할려고했어 근데 일케하면 충돌할때마다 렌더링할때 사용ㅇ했던 핼ㅇ렬을 가져와야했단말이야 그래서 싫어.
	// o : 콜라이더 복제시(객체에 컴포넌트로 추가될때)에 이 콜라이더가 변환되야하는 상태행렬의 정보(주소)를 담기위한 구조체.
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