#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_RcTex; // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
class CShader;
class CTexture;
_END

_BEGIN(Client)

class CBack_Logo final : public CGameObject
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBack_Logo(const CBack_Logo& rhs);
	virtual ~CBack_Logo() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*		m_pTransformCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
private:
	HRESULT			Add_Component();
public:
	static CBack_Logo*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END