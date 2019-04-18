#pragma once

#include "Component.h"

// ��ü�� ���¸� ǥ���ϱ����� Ŭ�����̴�.
// ex : ��ġ, ũ��, ȸ��.

_BEGIN(Engine)

class _DLL_EXPORTS CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:	// Getter
	// �ظ��ϸ� retrun �ϴ� Getter�� const�� �ٿ�����.
	const _matrix*	Get_WorldMatrixPointer() const { return &m_matWorld; }
	const _vec3*	Get_StateInfo(STATE eState) const { return (_vec3*)&m_matWorld.m[eState][0]; }
public:	// Setter
	void			Set_StateInfo(STATE eState, const _vec3* pStateInfo) const {
		memcpy((_vec3*)&m_matWorld.m[eState][0], pStateInfo, sizeof(_vec3));
	}
public:
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT	Set_Scaling(const _float& fX, const _float& fY, const _float& fZ);
	// vState�� right, up, look ���ͷ� � ���� �������� �������� �־��ش�.
	HRESULT	Set_Angle_Axis(_vec3 vState, const _float& fRadian);
	// iDirection�� 0�̸� Go, 1�̸� Back �̴�.
	HRESULT	Move(_int& iDirection, const _float& fSpeedPerSec, const _float& fTimeDelta);
	HRESULT	Rotation_Axis(_vec3 vState, const _float& fRadianPerSec, const _float& fTimeDelta);
	HRESULT Move_Target(const CTransform* pTransform, const _float& fSpeedPerSec, const _float& fTimeDelta);
public:
	HRESULT Ready_Transform();
private:
	_matrix	m_matWorld;
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END