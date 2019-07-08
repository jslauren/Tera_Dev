#pragma once
#include "Unit.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CUI_Dialog;
class CCamera_Static;
class CNPC : public CUnit
{
protected:
	explicit CNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNPC(const CNPC& rhs);
	virtual ~CNPC() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

protected:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	void			CollisionCheck(_bool _bIsPlayerInside = false, _bool _bIsLookPlayer = true);
	void			PlayerInside(const CCollider* _PlayerCollider);
	void			TalkEvent(const CCollider* _PlayerCollider, _bool _bIsLookPlayer);
	void			LookAtPlayer();
	void			TalkWithPlayer(_uint _iEndScriptNum, _uint _iLoopScriptNum = 1, _uint _iInitAniNum = 1, _bool _bIsQuestNPC = false, _float fResetViewAngle = 180.f);
	void			TalkEventFree(CUI_Dialog* _pUI_Dialog, _uint iAniNum, _float fResetViewAngle);
	virtual void	ScriptInfo();

protected:
	CCollider*		m_pColliderEventCom = nullptr;
	CCamera_Static*	m_pCameraStatic = nullptr;

protected:
	_float			m_fCulling = 5.f;
	_bool			m_bIsPlayerInside = false;
	_uint			m_iEventArgValue = 0;
	_bool			m_bPlayerRenderingFirst = true;
	_bool			m_bIsEventCollisionFirst = true;
	_bool			m_bIsTalking = false;
	_tchar*			m_pTitleScript = nullptr;
	_tchar*			m_pMainScript[10];
	_tchar*			m_pReplyScript[10];
	_int			m_iScriptNumber = -1;
	_bool			m_bIsTalkEnded = true;
	_bool			m_bIsArchitecture = false;
	_bool			m_bIsQuestNPC = false;
	_bool			m_bIsLoopOn = false;

public:
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();
};

_END