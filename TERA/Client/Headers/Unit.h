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
	enum UNIT_ID { UNIT_PLAYER, UNIT_MONSTER, UNIT_BOSSMONSTER, UNIT_END };
	enum STANCE_ID { STANCE_SEARCH, STANCE_TARCKING, STANCE_ATTACK, STANCE_SKILL, STANCE_GROGGY, STANCE_DEATH, STANCE_END };
	enum ACTION_ID 
	{
		ACTION_IDLE, ACTION_WALK, ACTION_RUN, ACTION_JUMP, ACTION_READY, 
		ACTION_CAST, ACTION_ATTACK, ACTION_PHYSICSSKILL, ACTION_STUN,
		ACTION_WOUND, ACTION_DIE, ACTION_END
	};

protected:
	explicit CUnit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUnit(const CUnit& rhs);
	virtual ~CUnit() = default;

public:	// Getter
	const ACTION_ID&	Get_ActionID() const { return m_eCurActionID; }
	const UNIT_ID&		Get_UnitID() const { return m_eUnitID; }
	const STANCE_ID&	Get_StanceID() const { return m_eCurStanceID; }

public:
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CTransform*		Get_TransformRotation() { return m_pTransformRotateCom; }
	CTransform*		Get_TransformMove() { return m_pTransformMoveCom; }
	CMesh_Dynamic*	Get_Mesh() { return m_pMeshCom; }
	CNavigation*	Get_NaviMesh() { return m_pNavigationCom; }

public:	// Setter
	void			Set_ActionID(const ACTION_ID& eActionID) { m_eCurActionID = eActionID; }
	void			Set_StanceID(const STANCE_ID& eStanceID) { m_eCurStanceID = eStanceID; }

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
	CTransform*		m_pTransformRotateCom = nullptr;
	CTransform*		m_pTransformMoveCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CNavigation*	m_pNavigationCom = nullptr;
	CFrustum*		m_pFrustumCom = nullptr;

//	CUnit*			m_pTargetUnit = nullptr;

	UNIT_ID			m_eUnitID;
	STANCE_ID		m_eCurStanceID = STANCE_SEARCH;
	STANCE_ID		m_ePreStanceID = STANCE_END;
	ACTION_ID		m_eCurActionID = ACTION_IDLE;
	ACTION_ID		m_ePreActionID = ACTION_END;

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