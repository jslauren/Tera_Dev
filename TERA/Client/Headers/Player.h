#pragma once
#include "Unit.h"
#include "Weapon.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer : public CUnit
{
public:
	enum PLAYER_ANI	// ÃÑ 42°³ÀÇ ¸ð¼Ç
	{
		RageRunner, Hit_Wake, Hit_Loop, Hit_Land, Hit_Rolling, Hit_High, Combo1, Combo1R, Combo2, Combo2R,
		Combo3, Combo3R, Combo4, CutHead, CuttingSlash, Death, DrawSwordCharge, DrawSwordLoop, DrawSwordMove, 
		DrawSword, DrawSwordEnd, FlatBlade, Groggy, HandySlash, InWeapon, OutWeapon, JawBreaker, RagingStrike,
		Run_Battle, Hit, StingerBlade, Tumbling, Idle_Battle, Fall, JumpStart, JumpLoop, JumpEndLand, JumpEnd,
		SitStart, SitLoop, SitEnd, Run, Idle, END
	};
	// Jump, Fall, Groggy, Death


private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:	// Getter
	CMesh_Dynamic_Bone*	Get_Mesh_Bone() { return m_pMeshCom_Bone; }
	const PLAYER_ANI&	Get_AniIndex() { return m_eAnimationIndex; }
	const PLAYER_ANI&	Get_OldAniIndex() { return m_eOldAnimationIndex; }
	const _bool&		Get_DrawSwordBtnState() { return m_bIsDrawSwordPressed; }
	const _int&			Get_Direction() { return m_iDirection; }
	const _bool&		Get_CutSceneInfo() { return m_bIsCutSceneEvent; }
	const _bool&		Get_CollisionCheckWhether() { return m_bCollisionCheck; }
	const _bool&		Get_DamageEventEndInfo() { return m_bDamageEventEndInfo; }
	const _float&		Get_HP() { return m_fHP; }
	const _float&		Get_MP() { return m_fMP; }
	const _uint&		Get_PlayerOffenceValue() { return m_iOffencePower; }
	const _bool&		Get_LBtnClickedInfo() { return m_bLBtnClicked; }
	const _bool&		Get_HitCount() { return m_iHitCount; }
	const _uint&		Get_CurrentCoolTimeInfo(PLAYER_ANI eAttackAni);
	const _bool&		Get_CoolTimeAvailable(PLAYER_ANI eAttackAni);

public:	// Setter
	HRESULT				Set_Navigation_Component(SCENEID eScene);
	void				Set_AniIndex(const PLAYER_ANI& iIndex) { m_eAnimationIndex = iIndex; }
	void				Set_OldAniIndex(const PLAYER_ANI& iIndex) { m_eOldAnimationIndex = iIndex; }
	void				Set_DrawSwordBtn(_bool bPressed) { m_bIsDrawSwordPressed = bPressed; }
	void				Set_Direction(_int iDir) { m_iDirection = iDir; }
	void				Set_CutSceneInfo(_bool bButton) { m_bIsCutSceneEvent = bButton; }
	void				Set_DamageEventEndInfo(_bool bButton) { m_bDamageEventEndInfo = bButton; }
	void				Set_HP_Sub(_float fDamageValue) { m_fHP -= fDamageValue; if (m_fHP <= 0.f) m_fHP = 0.f; }
	void				Set_MP_Sub(_float fDamageValue) { m_fMP -= fDamageValue; if (m_fMP <= 0.f) m_fMP = 0.f; }
	void				Set_HP_Add(_float fHealingValue) { m_fHP += fHealingValue; if (m_fHP >= 10686.f) m_fHP = 10686.f; }
	void				Set_MP_Add(_float fHealingValue) { m_fMP += fHealingValue; if (m_fMP >= 3250.f) m_fMP = 3250.f; }
	void				Set_PlayerOffenceValue(_uint iOffenceValue) { m_iOffencePower = iOffenceValue; }
	void				Set_LBtnClickedInfo(_bool bButton) { m_bLBtnClicked = bButton; }

	void				Set_AddHitCount() { m_iHitCount++; }
	void				Set_SubHitCount() { m_iHitCount--; }
	void				Set_FreeHitCount() { m_iHitCount = 0; }
	void				Set_CoolTimeAvailable(PLAYER_ANI eAttackAni, _bool bButton);
	void				Set_CoolTimeFree(PLAYER_ANI eAttackAni);

public:
	virtual HRESULT		Ready_GameObject_Prototype();
	virtual HRESULT		Ready_GameObject(void* pArg);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual _int		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT		Render_GameObject();
	virtual HRESULT		OnEvent(const _tchar * _szEventTag, void * _pMsg);

public:
	_bool				CollisionCheck();
	void				DamageEvent(_float fSpeed);
	void				DamageCalculator(PLAYER_ANI eAttackAni);
	void				DecreaseSkillCoolTime();

private:
	CPlayerState*			m_pState = nullptr;
	CMesh_Dynamic_Bone*		m_pMeshCom_Bone = nullptr;
	CMesh_Dynamic_Parts*	m_pMeshCom_Head = nullptr;
	CMesh_Dynamic_Parts*	m_pMeshCom_Body = nullptr;
	CMesh_Dynamic_Parts*	m_pMeshCom_Hand = nullptr;
	CMesh_Dynamic_Parts*	m_pMeshCom_Leg = nullptr;
	CMesh_Dynamic_Parts*	m_pMeshCom_Tail = nullptr;

private:
	const _matrix*	m_pBoneMatrix = nullptr;
	const _matrix*	m_pParentMatrix = nullptr;

	_vec3			m_vTargetPos;
	_float			m_fPlayerPosY = 0.f;

	PLAYER_ANI		m_eAnimationIndex = Idle;
	PLAYER_ANI		m_eOldAnimationIndex = Idle;

	_bool			m_bTest = false;

	_bool			m_bIsDrawSwordPressed = false;
	_int			m_iDirection = 1;

	_bool			m_bIsCutSceneEvent = false;

	_bool			m_bCollisionCheck = false;

	_bool			m_bDamageEventEndInfo = false;
	_bool			m_bIsNavigationRender = false;
	
	_float			m_fHP = 10686.f;
	_float			m_fMP = 3250.f;
	_float			m_fAutoHealingAccTime = 0.f;

	_uint			m_iOffencePower = 300.f;
	_bool			m_bIsCriticalDamage[8] = { false, false, false, false, false, false, false, false};

	_bool			m_bLBtnClicked = false;
	_uint			m_iHitCount = 0;

	_bool			m_bIsCoolTimeAvailable[9] = { true, true, true, true, true, true, true, true, true};
	_uint			m_iMaxCoolTime[9] = { 5, 4, 7, 3, 5, 3, 9, 5, 3};
	_uint			m_iCurrentCoolTime[9] = { 5, 4, 7, 3, 5, 3, 9, 5, 3 };

	_float			m_fCoolTimeAcc = 0.f;

private:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT			SetUp_HeightOnTerrain();
	void			ViewChanage();
	void			KeyInput();
	void			Compute_HeightOnNavi();
	void			AutoHealing(const _float& fTimeDelta);

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END