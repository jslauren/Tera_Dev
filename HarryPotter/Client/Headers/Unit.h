#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer; 
class CMesh_Dynamic;
class CShader;
class CCollider;
class CNavigation;
class CTexture;
_END

_BEGIN(Client)

class CUnit : public CGameObject
{
public:
	enum Unit_ID { Unit_Player, Unit_Monster, Unit_BossMonster, Unit_End };
	enum Stance_ID { Stance_Search, Stance_Tarcking, Stance_Attack, Stance_Skill, Stance_Groggy, Stance_Death, Stance_End };
	enum Action_ID 
	{
		Action_Idle, Action_Walk, Action_Run, Action_Jump, Action_Ready, 
		Action_Cast, Action_Attack, Action_PhysicsSkill, Action_Stun,
		Action_Wound, Action_Die, Action_End
	};

protected:
	explicit CUnit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUnit(const CUnit& rhs);
	virtual ~CUnit() = default;

public:	// Getter
	const Action_ID&	Get_ActionID() const { return m_eCurActionID; }
	const Unit_ID&		Get_UnitID() const { return m_eUnitID; }
	const Stance_ID&	Get_StanceID() const { return m_eCurStanceID; }

public:
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CMesh_Dynamic*	Get_Mesh() { return m_pMeshCom; }
	CNavigation*	Get_NaviMesh() { return m_pNavigationCom; }

public:	// Setter
	void			Set_ActionID(const Action_ID& eActionID) { m_eCurActionID = eActionID; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_HeightOnTerrain(_uint iIndex);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CNavigation*	m_pNavigationCom = nullptr;
//	CUnit*			m_pTargetUnit = nullptr;

	Unit_ID			m_eUnitID;
	Stance_ID		m_eCurStanceID = Stance_Search;
	Stance_ID		m_ePreStanceID = Stance_End;
	Action_ID		m_eCurActionID = Action_Idle;
	Action_ID		m_ePreActionID = Action_End;

	_float			m_fFrame = 0.f;
	_float			m_fAniSpeed = 1.f;
	_float			m_fTimeDelta = 1.f;
	_bool			m_bIsRun = false;
	_bool			m_bIsJumped = false;
	_float			m_fJumpTime = 0.f;

public:
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();

};

_END