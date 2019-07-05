#pragma once
#include "Base.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

// 클라이언트에서 생성할 씬들의 부모가 되는 클래스.

_BEGIN(Engine)

class CGameObject;
class _DLL_EXPORTS CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;
public:
	virtual HRESULT			Ready_Scene();
	virtual HRESULT			Init_Setting();
	virtual _int			Update_Scene(const _float& fTimeDelta);
	// 현재 씬의 Update호출 이후 최종적으로 한번 더 호출되는 Update
	// 씬안에서 객체들의 업데이트 호출. 모든 객체들의 업데이트를 호출한 후, 다시 한번 객체들의 업데이트를 호출.
	virtual _int			LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT			Render_Scene();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
protected:
	HRESULT Add_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag, CGameObject* pGameObject);
	HRESULT Add_Object(const _uint& iPorotoSceneIdx, const _tchar* pProtoTag, const _uint& iSceneIdx, const _tchar* pLayerTag, void* pArg = nullptr);
public:
	virtual void Free();
};

_END