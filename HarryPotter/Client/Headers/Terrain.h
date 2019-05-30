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

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
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
	//CTexture*			m_pFilterCom = nullptr;
	//CTexture*			m_pBrushCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
//private:
//	LPDIRECT3DTEXTURE9	m_pFilterTexture = nullptr;
private:
	D3DMATERIAL9		m_MtrlInfo;
	_vec4				m_vDetail = { 1.f, 1.f, 1.f, 1.f };
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	//HRESULT SetUp_RenderState();
	//HRESULT Release_RenderState();
public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();		
};

_END