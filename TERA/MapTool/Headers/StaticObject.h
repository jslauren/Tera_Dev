#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Static; 
class CRenderer;
//class CTexture;
_END

class CStaticObject final : public CGameObject
{
private:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
public:	// Setter
	void			SetState(_vec3 vPos, _vec3 vScale);
	void			SetPosition(_vec3 vPos);
	void			SetWorldMatrix(_matrix matWorld);
	void			SetNaviDotPos(_vec3 vNaviDotPos) { m_vNaviDotPos = vNaviDotPos; }
public:	// Getter
	_vec3			GetPosition(void) { return m_vPosition; }
	CTransform*		GetTransformCom() { return m_pTransformCom; }
	CMesh_Static*	GetMeshCom() { return m_pMeshCom; }
	_vec3			GetNaviDotPos() { return m_vNaviDotPos; }
public:
	HRESULT			Add_Component_Tool(const _tchar* pComponentPrototypeTag);
private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Static*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
//	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
private:
	HRESULT			Add_Component();
	HRESULT			SetUp_HeightOnTerrain();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_vec3			m_vPosition;
	_vec3			m_vNaviDotPos;
public:
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

