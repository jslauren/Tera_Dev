#pragma once

#include "Base.h"

// ��ü���� ��Ƽ� �����Ѵ�.
// �ΰ����� ��ü���� ����.(����, �ǻ���� ��ü)

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
private: // ������ü���� �����ϴ� �����̳�.
	map<const _tchar*, CGameObject*>*			m_pmapPrototype = nullptr;
	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;
private: // �� ������ �� ����� ��ü���� �����ϴ� �����̳�.	
	map<const _tchar*, CLayer*>*		m_pmapObject = nullptr;
	typedef map<const _tchar*, CLayer*>	MAPOBJECT;
private:
	CGameObject*	Find_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag);
	CLayer*			Find_Layer(const _uint& iSceneIdx, const _tchar* pLayerTag);
public:
	virtual void Free();
};

_END