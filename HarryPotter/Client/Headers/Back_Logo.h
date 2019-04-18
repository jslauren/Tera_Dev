#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_RcTex;	// �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer;		// ��ΰ� �����׷쿡 �߰��ϱ� ����. && Render�Լ��� ȣ���� �� �ֵ��� 
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
	virtual HRESULT	Ready_GameObject_Prototype();
	virtual HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	// ��� �� ������Ʈ ��ü���� ��� ������ ������ �ִ�.
	CTransform*		m_pTransformCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	HRESULT			Add_Component();
public:
	static CBack_Logo*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone();
	virtual void			Free();
};

_END