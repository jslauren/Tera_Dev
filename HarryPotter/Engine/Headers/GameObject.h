#pragma once
#include "Base.h"
#include "Component_Manager.h"

_BEGIN(Engine)

// 실 사용 객체든, 원본 객체든간에 모든 객체의 부모가 되는 클래스이다.

class _DLL_EXPORTS CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);	// 복사생성자
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
	// 이 클래스를 상속받는 자식 오브젝트 클래스들을,
	// 프로토 타입 패턴을 사용하여 복사 해주기 위해,
	// 그 자식 오브젝트들의 부모격인 이 클래스에서 Clone함수를 순수 가상함수로 정의하였다.
	// (순수 가상함수 정의 시, 모든 자식 클래스들은 해당 함수를 가지고 있어야 한다)
	virtual CGameObject*	Clone() = 0;
	virtual void			Free();
};

_END