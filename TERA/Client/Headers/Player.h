#pragma once
#include "Unit.h"
//#include "Defines.h"
//#include "GameObject.h"
#include "KeyManager.h"

_BEGIN(Engine)
//class CTransform;
//class CMesh_Dynamic; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
//class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
//class CCollider;
_END

_BEGIN(Client)

class CPlayerState;
class CPlayer : public CUnit
{
public:
	enum PLAYER_STATE	// 총 42개의 모션
	{
		R05UNARMEDWAIT, UNARMEDRUN, SITEND, SITLOOP, SITSTART, JUMPSTART, JUMPLOOP, JUMPENDLAND, JUMPEND, FALL,
		WAIT, STINGERBLADE, SREACTION, RUN, RISINGATTACK, JAWBREAKER, OUTWEAPON, INWEAPON, HANDYSLASH, GROGGY1,
		GAIACRUSH03, GAIACRUSH02, GAIACRUSH01, FLATBLADE, DRAWSWORDEND, DRAWSWORD, DRAWSWORDMOVE, DRAWSWORDLOOP,
		DRAWSWORDCHARGE, DEATH, CUTTINGSLASH, CUTHEAD, COMBO4, COMBO3R, COMBO3, COMBO2R, COMBO2, COMBO1R, COMBO1, 
		BREACTIONLAND3, BREACTIONCOM3, ADVLEAP, END
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:	// Getter
	const PLAYER_STATE&	Get_AniIndex() { return m_eAnimationIndex; }
	const PLAYER_STATE&	Get_OldAniIndex() { return m_eOldAnimationIndex; }
	const _bool			Get_BackBtnState() { return m_bIsFirstBackBtn; }

public:	// Setter
	void			Set_AniIndex(const PLAYER_STATE& iIndex) { m_eAnimationIndex = iIndex; }
	void			Set_OldAniIndex(const PLAYER_STATE& iIndex) { m_eOldAnimationIndex = iIndex; }
	void			Set_BackBtnState(_bool bState) { m_bIsFirstBackBtn = bState; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CPlayerState*	m_pState = nullptr;
	CMesh_Dynamic*	m_pMeshCom_PlayerFace = nullptr;

//private:
//	CTransform*		m_pTransformCom = nullptr;
//	CMesh_Dynamic*	m_pMeshCom = nullptr;
//	CRenderer*		m_pRendererCom = nullptr;
//	CCollider*		m_pBodyColliderCom = nullptr;
//	CCollider*		m_pHandColliderCom = nullptr;
//	CShader*		m_pShaderCom = nullptr;
private:
	const _matrix*	m_pBoneMatrix = nullptr;
	const _matrix*	m_pParentMatrix = nullptr;

	_vec3			m_vTargetPos;
	_float			m_fPlayerPosY = 0.f;

	PLAYER_STATE	m_eAnimationIndex = WAIT;
	PLAYER_STATE	m_eOldAnimationIndex = WAIT;
	_bool			m_bIsFirstBackBtn = true;

private:
	virtual HRESULT Add_Component();
	//HRESULT SetUp_HeightOnTerrain();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void			ViewChanage();
	void			KeyInput();

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END