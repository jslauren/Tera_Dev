#pragma once
#include "NPC.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CUI_Dialog;
class CCamera_Static;
class CCartNPC : public CNPC
{
private:
	explicit CCartNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCartNPC(const CCartNPC& rhs);
	virtual ~CCartNPC() = default;

public:	// Getter
	_bool			Get_SceneChangeAvailableInfo() { return m_bIsSceneChangeAvailable; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			ScriptInfo();

private:
	_int			m_iEndScriptNum = 1;
	_int			m_iLoopScriptNum = 1;
	_bool			m_bIsSceneChangeAvailable = false;

public:
	static CCartNPC*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END