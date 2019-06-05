#pragma once
#include "Defines.h"
#include "PlayerState.h"
#include "PlayerStateInfo.h"
#include "GameObject.h"
#include "KeyManager.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Dynamic; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
class CCollider;
_END

_BEGIN(Client)

class CPlayer : public CGameObject
{
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
//public:
//	virtual void	handleInput(P_STATE eInput);
//	virtual void	StateUpdate() { pState->StateUpdate(*this); }
//private:
//	CPlayerState*	pState;
private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCollider*		m_pBodyColliderCom = nullptr;
//	CCollider*		m_pHandColliderCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
private:
	CKeyManager*	m_pKeyManager = nullptr;
	_bool			m_isMove = false;
	_vec3			m_vTargetPos;
	_float			m_fAniSpeed = 1.f;
	_float			m_fTimeDelta = 1.f;
	_bool			m_bIsRun = false;
	_bool			m_bIsJumped = false;
	_float			m_fJumpTime = 0.f;
	_float			m_fPlayerPosY = 0.f;

	P_STATE			m_dwPlayerState = STATE_IDLE;
private:
	HRESULT Add_Component();
	HRESULT SetUp_HeightOnTerrain();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void	ViewChanage();
	void	KeyInput();
	void	AniChange();

	void	ETC();
public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END