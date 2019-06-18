#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_Terrain; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
class CTexture;
class CShader;
_END

_BEGIN(Client)

class CTerrain_Dragon final : public CGameObject
{
private:
	explicit CTerrain_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain_Dragon(const CTerrain_Dragon& rhs);
	virtual ~CTerrain_Dragon() = default;
public:

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CBuffer_Terrain*	m_pBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	D3DMATERIAL9		m_MtrlInfo;
	_vec4				m_vDetail = { 1.f, 1.f, 1.f, 1.f };
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CTerrain_Dragon*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();		
};

_END