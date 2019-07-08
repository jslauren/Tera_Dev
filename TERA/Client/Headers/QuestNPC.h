#pragma once
#include "NPC.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CUI_Dialog;
class CCamera_Static;
class CQuestNPC : public CNPC
{
public:
	enum QUEST_KINDS
	{
		Q_ARKUS, Q_OX, Q_DANCINGKING, Q_END
	};
	enum QUEST_STATE
	{
		QUEST_NONE, QUEST_START, QUEST_ONGOING, QUEST_REWARD, QUEST_END
	};
private:
	explicit CQuestNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CQuestNPC(const CQuestNPC& rhs);
	virtual ~CQuestNPC() = default;

public:	// Getter
	QUEST_STATE		GetCurrentQuestStateInfo() { return m_eCurrentQuestState; }

public:	// Setter
	void			SetCurrentQuestStateInfo(QUEST_STATE eCurrentState) { m_eCurrentQuestState = eCurrentState; }

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
	QUEST_STATE		m_eCurrentQuestState = QUEST_START;
	QUEST_KINDS		m_eCurrentQeustKinds = Q_ARKUS;

public:
	static CQuestNPC*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END