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
		Idle, Apperance01, Run_Battle, RoundAtk02, RoundAtk01,
		MoveAtkStart, MoveAtkLoop, MoveAtkEnd, JumpEvasion,
		HeavyAtk02, Groggy, Hit, Death, Atk01, Apperance02, End
	};

public:	// Getter
	const ARKUS_ANI&	Get_AniIndex() { return m_eAnimationIndex; }
	const ARKUS_ANI&	Get_OldAniIndex() { return m_eOldAnimationIndex; }
	CMesh_Dynamic*		Get_Mesh() { return m_pMeshCom; }

public:	// Setter
	void			Set_AniIndex(const ARKUS_ANI& iIndex) { m_eAnimationIndex = iIndex; }
	void			Set_OldAniIndex(const ARKUS_ANI& iIndex) { m_eOldAnimationIndex = iIndex; }

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
	
	void			CollisionCheck();
	void			AI();

//protected:
//	Engine::OBJECTMESHDATA tObjectMeshData;
private:
	CCollider*		m_pColliderHeadCom = nullptr;
	CCollider*		m_pColliderNeckCom = nullptr;
	CCollider*		m_pColliderTail01Com = nullptr;
	CCollider*		m_pColliderTail02Com = nullptr;

private:
	CArkusState*	m_pState = nullptr;

private:
	ARKUS_ANI		m_eAnimationIndex = Idle;
	ARKUS_ANI		m_eOldAnimationIndex = Idle;

private:
	_bool			m_bCollisionPartsCheck[6];
	_bool			m_bCollisionCheck = false;

public:
	static CArkus*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END