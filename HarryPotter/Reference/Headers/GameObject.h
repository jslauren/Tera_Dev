#pragma once
#include "Base.h"
#include "Component_Manager.h"

_BEGIN(Engine)

// �� ��� ��ü��, ���� ��ü�簣�� ��� ��ü�� �θ� �Ǵ� Ŭ�����̴�.

class _DLL_EXPORTS CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);	// ���������
	virtual ~CGameObject() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
protected:
	HRESULT Add_Component(const _uint& iSceneIdx, const _tchar* pComponentTag, CComponent** ppOutComponent);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
public:
	// �� Ŭ������ ��ӹ޴� �ڽ� ������Ʈ Ŭ��������,
	// ������ Ÿ�� ������ ����Ͽ� ���� ���ֱ� ����,
	// �� �ڽ� ������Ʈ���� �θ���� �� Ŭ�������� Clone�Լ��� ���� �����Լ��� �����Ͽ���.
	// (���� �����Լ� ���� ��, ��� �ڽ� Ŭ�������� �ش� �Լ��� ������ �־�� �Ѵ�)
	virtual CGameObject*	Clone() = 0;
	virtual void			Free();
};

_END