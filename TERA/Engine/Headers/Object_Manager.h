#pragma once

#include "Base.h"

// ��ü���� ��Ƽ� �����Ѵ�.
// �ΰ����� ��ü���� ����.(����, �ǻ���� ��ü)

_BEGIN(Engine)

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
private:
	_uint	m_iMaxNumScene = 0;

private: // ������ü���� �����ϴ� �����̳�.
	map<const _tchar*, CGameObject*>*			m_pmapPrototype = nullptr;
	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;
private:
	CGameObject* Find_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag);

public:
	virtual void Free();
};

_END