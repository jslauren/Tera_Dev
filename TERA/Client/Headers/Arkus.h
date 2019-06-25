#pragma once
#include "Unit.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CArkusState;
class CArkus final : public CUnit
{
public:
	enum ARKUS_ANI	// ÃÑ 14°³ÀÇ ¸ð¼Ç
	{
		AlmostDead, FlyAtk02End, FlyAtk02Start, FlyAtk01, Idle, 
		Apperance01, Run_Battle, RoundAtk02, RoundAtk01,
		MoveAtkStart, MoveAtkLoop, MoveAtkEnd, JumpEvasion,
		HeavyAtk02, Groggy, Hit, Death, Atk01, Apperance02, End
	};
	enum ARKUS_COLLISION
	{
		COLL_BOOY, COLL_HEAD, COLL_NECK, 
		COLL_TAIL01, COLL_TAIL02, COLL_ATTACK_AREA, COLL_END
	};

public:	// Getter
	const ARKUS_ANI&	Get_AniIndex() { return m_eAnimationIndex; }
	const ARKUS_ANI&	Get_OldAniIndex() { return m_eOldAnimationIndex; }
	CMesh_Dynamic*		Get_Mesh() { return m_pMeshCom; }
	_bool				Get_CollisionCheck() { return m_bCollisionCheck; }
	_bool				Get_CollisionPartCheck(ARKUS_COLLISION eCollisionPart) { return m_bCollisionPart[eCollisionPart]; }
	_bool				Get_PlayerFrontInfo() { return m_bIsPlayerFront; }
	_bool				Get_TurnRightInfo() { return m_bIsTurnRight; }

public:	// Setter
	void			Set_AniIndex(const ARKUS_ANI& iIndex) { m_eAnimationIndex = iIndex; }
	void			Set_OldAniIndex(const ARKUS_ANI& iIndex) { m_eOldAnimationIndex = iIndex; }
	void			Set_PlayerFrontInfo(_bool bButton) { m_bIsPlayerFront = bButton; }
	void			Set_TurnRightInfo(_bool bButton) { m_bIsTurnRight = bButton; }

public:
	explicit CArkus(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CArkus(const CArkus& rhs);
	virtual ~CArkus() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT			SetUp_HeightOnTerrain(_uint iIndex);
	
	_bool			CollisionCheck();
	void			CollisionCheck_Attack_Area();
	void			ViewChanage();
	void			AI();
	_bool			EnemyPositionCheck();
	void			LookChangeToPlayer(_bool bPtoM);

private:
	CCollider*		m_pColliderHeadCom = nullptr;
	CCollider*		m_pColliderNeckCom = nullptr;
	CCollider*		m_pColliderTail01Com = nullptr;
	CCollider*		m_pColliderTail02Com = nullptr;
	CCollider*		m_pColliderAtkAreaCom = nullptr;

private:
	CArkusState*	m_pState = nullptr;

private:
	ARKUS_ANI		m_eAnimationIndex = Idle;
	ARKUS_ANI		m_eOldAnimationIndex = Idle;

private:
	_vec3			m_vPlayerDir;
	_float			m_fDirAngle = 0.f;
	
	_bool			m_bCollisionCheck = false;

	_bool			m_bCollisionPart[COLL_END] = { false, false, false, false, false, false };

	_bool			m_bCollisionBody = false;
	_bool			m_bCollisionHead = false;
	_bool			m_bCollisionNeck = false;
	_bool			m_bCollisionTail01 = false;
	_bool			m_bCollisionTail02 = false;
	_bool			m_bCollisionAttackArea = false;

	_bool			m_bIsPlayerFront = true;
	_bool			m_bIsTurnRight = false;

public:
	static CArkus*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END