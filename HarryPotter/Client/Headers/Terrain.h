#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_Terrain; // �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer; //��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
class CTexture;
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
private:
	D3DMATERIAL9		m_MtrlInfo;
private:
	HRESULT Add_Component();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();		
};

_END