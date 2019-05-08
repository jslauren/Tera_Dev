#pragma once
#include "Base.h"

// 객체들을 모아서 관리한다.
// 두가지의 객체들을 보관.(원본, 실사용할 객체)

_BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class _DLL_EXPORTS CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	const CComponent* Get_Component(const _uint& iSceneIdx, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex = 0);
public:
	//객체들을 보관하는 맵들의 동적 배열의 크기를 정해준다,
	HRESULT Reserve_Object_Manager(const _uint& iMaxNumScene);
	
	//원본 객체를 맵에 삽입
	HRESULT Add_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag, CGameObject* pInGameObject);
	
	//클론 객체를 생성
	HRESULT Add_Object(const _uint& iProtoSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, void* pArg = nullptr);

	//클론 객체를 삭제
	HRESULT Clear_Prototype(const _uint& iSceneIdx);

	//원본 객체를 삭제
	HRESULT Clear_Object(const _uint& iSceneIdx);

	_int	Update_Object_Manager(const _float& fTimeDelta);
	_int	LateUpdate_Object_Manager(const _float& fTimeDelta);
public:
	// 복사본맵에서 오브젝트 레이어를 검색
	CLayer*				FindObjectLayer(const _uint& iSceneIdx, const _tchar* pLayerTag);
	const CComponent*	GetComponent(const _uint& iSceneIdx, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex = 0);
private:
	_uint	m_iMaxNumScene = 0;

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
	virtual void	Free();
};

_END