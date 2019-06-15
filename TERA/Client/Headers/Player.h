#pragma once
#include "Unit.h"
#include "KeyManager.h"
#include "Weapon.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer : public CUnit
{
public:
	enum PLAYER_ANI	// ÃÑ 42°³ÀÇ ¸ð¼Ç
	{
		RageRunner, Hit_Wake, Hit_Loop, Hit_Land, Hit_Rolling, Hit_High, Combo1, Combo1R, Combo2, Combo2R,
		Combo3, Combo3R, Combo4, CutHead, CuttingSlash, Death, DrawSwordCharge, DrawSwordLoop, 
		DrawSwordMove, DrawSword, DrawSwordEnd, FlatBlade, Groggy, HandySlash, InWeapon, OutWeapon, JawBreaker, 
		RagingStrike, Run_Battle, Hit, StingerBlade, Tumbling, Idle_Battle, Fall, JumpStart, JumpLoop, JumpEndLand, JumpEnd,
		SitStart, SitLoop, SitEnd, Run, Idle, END
	};
	// Jump, Fall, Tumbling_, RagingStrike_, Groggy, Death, Hit_Rolling_
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:	// Getter
	const PLAYER_ANI&	Get_AniIndex() { return m_eAnimationIndex; }
	const PLAYER_ANI&	Get_OldAniIndex() { return m_eOldAnimationIndex; }
	const _bool&		Get_DrawSwordBtnState() { return m_bIsDrawSwordPressed; }
	CMesh_Dynamic_Bone*	Get_Mesh_Bone() { return m_pMeshCom_Bone; }

public:	// Setter
	void			Set_AniIndex(const PLAYER_ANI& iIndex) { m_eAnimationIndex = iIndex; }
	void			Set_OldAniIndex(const PLAYER_ANI& iIndex) { m_eOldAnimationIndex = iIndex; }
	void			Set_DrawSwordBtn(_bool bPressed) { m_bIsDrawSwordPressed = bPressed; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

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
	
private:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT			SetUp_HeightOnTerrain();
	void			ViewChanage();
	void			KeyInput();
	void			Compute_HeightOnNavi();

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END