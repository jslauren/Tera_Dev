#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "KeyManager.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Dynamic; // �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer; //��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
class CTexture;
_END

_BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum STATE { STATE_END };
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	CTransform*		m_pTransformCom = nullptr;
	CMesh_Dynamic*	m_pMeshCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
private:
	CKeyManager*	m_pKeyManager = nullptr;
	_bool			m_isMove = false;
	_vec3			m_vTargetPos;
	_float			m_fAniTime = 1.f;
private:
	HRESULT Add_Component();
	HRESULT SetUp_HeightOnTerrain();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void	ViewChanage();

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END