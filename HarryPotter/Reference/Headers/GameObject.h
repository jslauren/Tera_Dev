#pragma once
#include "Base.h"
#include "KeyManager.h"
#include "EventManager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

// �� ��� ��ü��, ���� ��ü�簣�� ��� ��ü�� �θ� �Ǵ� Ŭ�����̴�.

_BEGIN(Engine)

class _DLL_EXPORTS CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	const CComponent*	Get_Component(const _tchar* pComponentTag);
	_float				Get_ViewZ() const { return m_fViewZ; }
public:
	void Set_Transform(_D3DTRANSFORMSTATETYPE eType, const _matrix* pMatrix) { 
						m_pGraphic_Device->SetTransform(eType, pMatrix); }
	void Set_SamplerState(_ulong dwSampler, D3DSAMPLERSTATETYPE SamplerState, _ulong dwValue);
	void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwValue);
	void Set_Material(const D3DMATERIAL9& Material);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

	//�̺�Ʈ �߻��� ȣ��Ǵ� �Լ�
	//1. ��̺�Ʈ����, 2. �߽��ڷκ��Ϳ� �޼���
	//�ش� �̺�Ʈ���� ����� ����ϰ���� ��� ������ ��ȯ�ϸ� �ȴ�. 
	//��ü�� �����Ǿ�� �Ҷ��� ������ ���� ��ȯ�� �Ͽ� ����� ����Ͽ� ��ü�� ����� �����ȴ�.(���۷��� ī���� ������)
	//���� �̺�Ʈ�Լ� �ȿ����� �ٸ� �̺�Ʈ�� �߻���Ű�� �ȵȴ�.(���� ���� ���ɼ�)
	virtual _int OnEvent(const _tchar* _pSubject, void* _pMsg);
public:
	//������Ʈ �Ŵ����� ��ü�� ���� �Ǵ��� �ϱ� ���� �ʿ��� �Լ�
	const bool& GetDelete();
public:
	CComponent*			Find_Component(const _tchar* pComponentTag);
	const CComponent*	GetComponent(const _tchar* pComponentTag);
protected:
	HRESULT				Add_Component(const _uint& iSceneIdx, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOutComponent);
	HRESULT				Compute_ViewZ(CTransform* pTransform);
protected:	// Protected�� �ø��� ����....
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CComponent_Manager*	m_pComponent_Manager = nullptr;
private:
	// CGameObject�� ��ӹ��� ��ü�� ������ �ִ� ������Ʈ�� ��Ƴ������� �����̳�.
	map<const _tchar*, CComponent*>			m_mapComponents;
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENTS;
protected:
	// �ش� ������ Ʈ���� ��� Ŭ���̴�.
	_bool	m_isClone = false;
	// �ش� ������ Ʈ�簡 �ɰ�� ������Ʈ �Ŵ������� ��ü�� �����Ѵ�.
	_bool	m_bDelete = false;
private:
	_float	m_fViewZ = 0.f;
public:
	// �� Ŭ������ ��ӹ޴� �ڽ� ������Ʈ Ŭ��������,
	// ������ Ÿ�� ������ ����Ͽ� ���� ���ֱ� ����,
	// �� �ڽ� ������Ʈ���� �θ���� �� Ŭ�������� Clone�Լ��� ���� �����Լ��� �����Ͽ���.
	// (���� �����Լ� ���� ��, ��� �ڽ� Ŭ�������� �ش� �Լ��� ������ �־�� �Ѵ�)
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();
};

_END