#pragma once
#include "Unit.h"
//#include "Defines.h"
//#include "GameObject.h"
#include "KeyManager.h"

//_BEGIN(Engine)
//class CTransform;
//class CMesh_Dynamic; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
//class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
//class CCollider;
//_END

_BEGIN(Client)

class CPlayerState;
class CPlayer : public CUnit
{
	enum PLAYER_STATE
	{
		LUMOSSTRAFERIGHT, LUMOSSTRAFELEFT, LUMOSSTRAFEFORWARD, LUMOSSTRAFEBACK,
		JUMPLANDTORUN, ACQUIREWIZARDCARD, ATTACKHITFALL, ATTACKHITSTAND, BATTLECAST,
		BATTLEIDLE,	BUNDIJUMP1, BUNDIJUMP2, CAST1, CASTAIM, CLIMB32, CLIMB64, CLIMB96START,
		CLIMB96END, COLLAPSE, CR0CASTAIM, CR1CATCH, CR2FLY, CR3APEX, CR4FALL, CR6DANGLE, CRAWLCROUCH,
		CRAWLCIRCULARFORWARD, CRAWLSTAND, CRPULL, FAINT, IDLE, JUMP_START, JUMP, JUMPLANDTOSTAND, LADDERBOTTOMMOUNTDISMOUNT,
		LADDERDOWNMOUNT, LADDERUPDISMOUNT, LADDERUPDOWN, PANELPUSH, PROPHOLDIDLE, PROPHOLDWALK, PROPHOLDWALKBACK, PROPHOLDWALKLEFT,
		PROPHOLDWALKRIGHT, PROPPICKUP, PROPTHROW, RUN, RUN_BACK, RUN_LEFT, RUN_RIGHT, RUNTOSTOP, SEARCHCHEST, SEARCHCHESTSTAND, STANDTOWALKNONE, WALK,
		END
	};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CPlayerState*	m_pState = nullptr;
//private:
//	CTransform*		m_pTransformCom = nullptr;
//	CMesh_Dynamic*	m_pMeshCom = nullptr;
//	CRenderer*		m_pRendererCom = nullptr;
//	CCollider*		m_pBodyColliderCom = nullptr;
////	CCollider*		m_pHandColliderCom = nullptr;
//	CShader*		m_pShaderCom = nullptr;
private:
	CKeyManager*	m_pKeyManager = nullptr;
	_bool			m_isMove = false;
	_vec3			m_vTargetPos;
	_float			m_fPlayerPosY = 0.f;
private:
	virtual HRESULT Add_Component();
	//HRESULT SetUp_HeightOnTerrain();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void	ViewChanage();
	void	KeyInput();

	void	ETC();
public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END