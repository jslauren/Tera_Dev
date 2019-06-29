#pragma once
#include "Base.h"
#include "KeyManager.h"
#include "EventManager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

// 실 사용 객체든, 원본 객체든간에 모든 객체의 부모가 되는 클래스이다.

_BEGIN(Engine)

class _DLL_EXPORTS CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	LPDIRECT3DDEVICE9	Get_Graphic_Device() const { return m_pGraphic_Device; }
	const CComponent*	Get_Component(const _tchar* pComponentTag);
	_float				Get_ViewZ() const { return m_fViewZ; }
	_matrix				Get_Transform(_D3DTRANSFORMSTATETYPE eType) const;
	_int				Get_IdxNum() const { return m_iIdxNum; }
public:
	void Set_Transform(_D3DTRANSFORMSTATETYPE eType, const _matrix* pMatrix) { m_pGraphic_Device->SetTransform(eType, pMatrix); }
	void Set_SamplerState(_ulong dwSampler, D3DSAMPLERSTATETYPE SamplerState, _ulong dwValue);
	void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwValue);
	void Set_Material(const D3DMATERIAL9& Material);
	void SetIdxNum(_int iIdx) { m_iIdxNum = iIdx; }
public:
	_bool	Picking(HWND hWnd, CTransform * pTransform, LPD3DXBASEMESH pMesh, _vec3 * pOut, _float* fFinalDist);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
	virtual HRESULT	OnEvent(const _tchar * _szEventTag, void * _pMsg);

public:
	//오브젝트 매니져가 객체의 삭제 판단을 하기 위해 필요한 함수
	const bool&		GetDelete();

protected:
	HRESULT	Add_Component(const _uint& iSceneIdx, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOutComponent, void* pArg = nullptr);
	HRESULT	Compute_ViewZ(CTransform* pTransform);

protected:	// Protected로 시마이 하자....
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CComponent_Manager*	m_pComponent_Manager = nullptr;

protected:
	// 해당 변수가 트루일 경우 클론이다.
	_bool	m_isClone = false;
	// 해당 변수가 트루가 될경우 오브젝트 매니져에서 객체를 삭제한다.
	_bool	m_bDelete = false;

	_float	m_fViewZ = 0.f;

	// 맵툴 트리컨트롤 인덱스용 변수.
	_int	m_iIdxNum = 0;

private:
	// CGameObject를 상속받은 객체가 가지고 있는 컴포넌트를 모아놓기위한 컨테이너.
	map<const _tchar*, CComponent*>			m_mapComponents;
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENTS;

private:
	CComponent*			Find_Component(const _tchar* pComponentTag);

public:
	// 이 클래스를 상속받는 자식 오브젝트 클래스들을,
	// 프로토 타입 패턴을 사용하여 복사 해주기 위해,
	// 그 자식 오브젝트들의 부모격인 이 클래스에서 Clone함수를 순수 가상함수로 정의하였다.
	// (순수 가상함수 정의 시, 모든 자식 클래스들은 해당 함수를 가지고 있어야 한다)
	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free();
};

_END