#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_CubeTex;	// �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer;		// ��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
class CTexture;
class CShader;
_END

_BEGIN(Client)

class CSkyBox_Dragon final : public CGameObject
{
private:
	explicit CSkyBox_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkyBox_Dragon(const CSkyBox_Dragon& rhs);
	virtual ~CSkyBox_Dragon() = default;
public:

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CBuffer_CubeTex*	m_pBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CSkyBox_Dragon*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END