#pragma once

#include "Base.h"

// 객체들을 모아서 관리한다.
// 두가지의 객체들을 보관.(원본, 실사용할 객체)

_BEGIN(Engine)

class CLayer;
class CGameObject;
class _DLL_EXPORTS CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	HRESULT Reserve_Object_Manager(const _uint& iMaxNumScene);
	HRESULT Add_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag, CGameObject* pInGameObject);
	HRESULT Clear_Object_Prototype(const _uint& iSceneIdx);
	HRESULT Add_Object(const _uint& iProtoSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, void* pArg = nullptr);
	_int	Update_Object_Manager(const _float& fTimeDelta);
	_int	LateUpdate_Object_Manager(const _float& fTimeDelta);
private:
	_uint		m_iMaxNumScene = 0;
private: // 원본객체들을 보관하는 컨테이너.
	map<const _tchar*, CGameObject*>*			m_pmapPrototype = nullptr;
	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;
private: // 각 씬에서 실 사용할 객체들을 보관하는 컨테이너.	
	map<const _tchar*, CLayer*>*		m_pmapObject = nullptr;
	typedef map<const _tchar*, CLayer*>	MAPOBJECT;
private:
	CGameObject*	Find_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag);
	CLayer*			Find_Layer(const _uint& iSceneIdx, const _tchar* pLayerTag);
public:
	virtual void Free();
};

_END