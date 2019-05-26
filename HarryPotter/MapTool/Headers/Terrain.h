#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"
#include "EventManagerTool.h"
#include "Buffer_Terrain_Tool.h"
#include "Texture_Tool.h"

_BEGIN(Engine)
class CTransform;
 // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록
class CTexture;
class CShader;
_END

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public: // Setter
	void	SetFillMode(_bool bIsWireFrame);

public:	// Getter
	CTransform*		GetTransformCom() { return m_pTransformCom; }
	CBuffer_Terrain_Tool* GetBufferCom() { return m_pBufferCom; }

public:
	HRESULT	Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail);
	HRESULT	Reset_Texture(_tchar* pFilePath);
	_bool	Picking(HWND hWnd, CTransform* pTransform, _vec3* pOut);

private:
	CTransform*				m_pTransformCom = nullptr;
	CBuffer_Terrain_Tool*	m_pBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture_Tool*			m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private:
	_int					m_iPassNum = 0;
	D3DMATERIAL9			m_MtrlInfo;
	_vec4					m_vDetail = { 1.f, 1.f, 1.f, 1.f };
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

