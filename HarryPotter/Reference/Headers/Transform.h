#pragma once
#include "Component.h"

// 객체의 상태를 표현하기위한 클래스이다.
// ex : 위치, 크기, 회전.

_BEGIN(Engine)

class _DLL_EXPORTS CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public: // Getter
	// 왠만하면 retrun 하는 Getter는 const를 붙여주자.
	const _matrix*	Get_WorldMatrixPointer() const { return &m_matWorld; }
	const _vec3*	Get_StateInfo(STATE eState) const { return (_vec3*)&m_matWorld.m[eState][0]; }
	const _vec3*	Get_RotRadValue() const { return &m_vRotRadValue; }
public: // Setter
	void			Set_StateInfo(STATE eState, const _vec3* pStateInfo) const { memcpy((_vec3*)&m_matWorld.m[eState][0], pStateInfo, sizeof(_vec3)); }
public:
	const _matrix*	Compute_InverseWorldMatrixPointer() { return D3DXMatrixInverse(&m_matWorldInv, nullptr, &m_matWorld); }

	HRESULT	SetUp_OnGraphicDev();
	HRESULT	Set_Scaling(const _float& fX, const _float& fY, const _float& fZ);
	HRESULT Set_Rotation_YawPitchRoll(_float fRadianY, _float fRadianX, _float fRadianZ);

	// vState에 right, up, look 벡터로 어떤 축을 기준으로 돌릴껀지 넣어준다.
	HRESULT	Set_Angle_Axis(_vec3 vState, const _float& fRadian);

	// 0, 1, 2, 3 - Up, Down, Left, Right
	HRESULT	Move(_int iDirection, const _float& fSpeedPerSec, const _float& fTimeDelta);
	HRESULT	Rotation_Axis(const _vec3& vAxis, const _float& fRadianPerSec, const _float& fTimeDelta);
	HRESULT	Move_Target(const CTransform* pTransform, const _float& fSpeedPerSec, const _float& fTimeDelta);
	HRESULT	Move_Target(const _vec3* pTargetPos, const _float& fSpeedPerSec, const _float& fTimeDelta, _bool* pFinish);

	//HRESULT	Set_Angle_X(const _float& fRadian);
	//HRESULT	Set_Angle_Y(const _float& fRadian);
	//HRESULT	Set_Angle_Z(const _float& fRadian);
	//HRESULT	Go_Straight(const _float& fSpeedPerSec, const _float& fTimeDelta);
	//HRESULT	Back_Straight(const _float& fSpeedPerSec, const _float& fTimeDelta);
	//HRESULT	Move_Right(const _float& fSpeedPerSec, const _float& fTimeDelta);
	//HRESULT	Move_Left(const _float& fSpeedPerSec, const _float& fTimeDelta);
	//HRESULT	Rotation_X(const _float& fRadianPerSec, const _float& fTimeDelta);
	//HRESULT	Rotation_Y(const _float& fRadianPerSec, const _float& fTimeDelta);
	//HRESULT	Rotation_Z(const _float& fRadianPerSec, const _float& fTimeDelta);
public:
	HRESULT Ready_Transform();
private:
	_matrix	m_matWorld;
	_matrix	m_matWorldInv;
	_vec3	m_vRotRadValue = { 0.f, 0.f, 0.f };
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END