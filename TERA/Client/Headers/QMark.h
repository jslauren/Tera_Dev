#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Dynamic;
class CRenderer;
class CShader;
_END

_BEGIN(Client)

class CQMark final : public CGameObject
{
public:
	enum QMARK_KINDS
	{
		QMARK_START, QMARK_ONGOING, QMARK_REWARD, QMARK_NONE, QMARK_END
	};

private:
	explicit CQMark(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CQMark(const CQMark& rhs);
	virtual ~CQMark() = default;

public:	// Getter
	const QMARK_KINDS&		Get_CurrentMark() { return m_eCurrentMark; }

public:	// Setter
	void					Set_CurrentMark(QMARK_KINDS eMark) { m_eCurrentMark = eMark; }

public:
	virtual HRESULT			Ready_GameObject_Prototype();
	virtual HRESULT			Ready_GameObject(void* pArg);
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT			Render_GameObject();

private:
	virtual HRESULT			Add_Component();
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CMesh_Dynamic*			m_pMeshQMarkCom[3] = { nullptr, nullptr, nullptr };

private:
	_float					m_fFrame = 0.f;
	_float					m_fTimeDelta = 0.0f;
	QMARK_KINDS				m_eCurrentMark = QMARK_START;

public:
	static CQMark*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END