#include "..\Headers\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

const CComponent * CObject_Manager::Get_Component(const _uint & iSceneIdx, const _tchar * pLayerTag, const _tchar * pComponentTag, const _uint & iIndex)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iSceneIdx, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

CGameObject * CObject_Manager::Get_Object(const _uint & iSceneIdx, const _tchar * pLayerTag, const _uint & iIndex)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iSceneIdx, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	list<CGameObject*>* pTempObjList = &pLayer->Get_ObjectList();

	if (iIndex == -1)
		return pTempObjList->back();
	
	auto iter = pTempObjList->begin();

	for (size_t i = 0; i < iIndex; i++)
	{
		++iter;
	}
	
	return *iter;
}

HRESULT CObject_Manager::Reserve_Object_Manager(const _uint & iMaxNumScene)
{
	// 쉽게 생각하면 Ready_Object_Manager 정도가 아닐까 생각된다.

	if (nullptr != m_pmapPrototype ||
		nullptr != m_pmapObject)
		return E_FAIL;

	// 여기는 Engine이고 Client에서 이것들을 사용 할 터인데..
	// 클라는 제각각이고 고정된 값을 줄 수 없어서
	// 씬 갯수를 지정하고, 씬별로 오브젝트들을 따로 관리하기위해 요렇게 진행한다.
	m_pmapPrototype = new MAPPROTOTYPE[iMaxNumScene];
	m_pmapObject = new MAPOBJECT[iMaxNumScene];
	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag, CGameObject * pInGameObject)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype ||
		nullptr == pInGameObject)
		return E_FAIL;

	// 원본 객체를 추가하기 전에 해당 객체가 이미 존재하는지 먼저 확인하는 함수를 실행한다.
	CGameObject*	pGameObject = Find_Object_Prototype(iSceneIdx, pProtoTag);

	// 해당 객체가 존재하면... ㅈ된거라고..?
	// 그냥 이미 존재하니깐 실패로 리턴해주는 것 같은데..
	// 메시지 박스를 띄어주는게 좋지 않을까?
	if (nullptr != pGameObject)
		return E_FAIL;

	// 이걸 이렇게 해주는 이유는,
	// 받아온 _tchar 포인터의 pProtoTag가 리터럴 상수로 하면 (L"" 이런거)는 문제가 없는데,
	// 걍 변수로 때려박아 버리면 문제가 생긴다. 변수 안에 있는 포인터가 가르키는 값이 변할때마다 ㅈ되버린다. 이른바 얕은복사와 비슷한 상황.
	// 그래서 이렇게 동적배열을 지정해줘서 넣어준다.
	_tchar* dynamicArray = new _tchar[lstrlen(pProtoTag) + 1];
	lstrcpy(dynamicArray, pProtoTag);

	//원본 객체들을 보관하는 컨테이너에 인자로 받은 게임 오브젝트를 꽂아 넣어 줍니다.
	m_pmapPrototype[iSceneIdx].insert(MAPPROTOTYPE::value_type(dynamicArray, pInGameObject));

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object(const _uint & iProtoSceneID, const _tchar * pProtoTag, const _uint & iSceneID, const _tchar * pLayerTag, void* pArg)
{
	// 먼저 해당 오브젝트가 원본 객체에 존재하는지 검사하고
	CGameObject*	pPrototype = Find_Object_Prototype(iProtoSceneID, pProtoTag);

	// 없다면.. ㅈ된거쥬..?
	if (nullptr == pPrototype)
		return E_FAIL;

	// 있다면 해당 원본객체를 복사 조져버린다. 이때 등장하는게 Clone 되시겠다.
	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	// 인자로 들어온 값이 실사용 객체로 존재하는지 확인하고
	CLayer*		pLayer = Find_Layer(iSceneID, pLayerTag);

	// 레이어가 없었다면,
	// 레이어를 만들어서 객체를 추가 후, 레이어를 추가한다.
	if (nullptr == pLayer) 
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		// CLayer Class 에 m_ObjectList 리스트에 push_back 한다.
		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
			return E_FAIL;

		_tchar* dynamicArray = new _tchar[lstrlen(pLayerTag) + 1];
		lstrcpy(dynamicArray, pLayerTag);

		m_pmapObject[iSceneID].insert(MAPOBJECT::value_type(dynamicArray, pLayer));
	}
	else // 있었다면 그냥 추가한다.
	{
		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CObject_Manager::Clear_Object(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return E_FAIL;

	for (auto& ObjectLayerPair : m_pmapObject[iSceneIdx])
	{
		_tchar* pTempArray = const_cast<_tchar*>(ObjectLayerPair.first);
		Safe_Delete_Array(pTempArray);
		Safe_Release(ObjectLayerPair.second);
	}

	m_pmapObject[iSceneIdx].clear();

	return NOERROR;
}

HRESULT CObject_Manager::Clear_Prototype(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype)
		return E_FAIL;

	for (auto& ProtypePair : m_pmapPrototype[iSceneIdx])
	{
		_tchar* pTempArray = const_cast<_tchar*>(ProtypePair.first);
		Safe_Delete_Array(pTempArray);
		Safe_Release(ProtypePair.second);
	}

	m_pmapPrototype[iSceneIdx].clear();

	return NOERROR;
}

_int CObject_Manager::Update_Object_Manager(const _float & fTimeDelta)
{
	if (nullptr == m_pmapObject)
		return -1;
	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxNumScene; ++i)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			// 실 사용 객체들의 ObjectList에 들어있는 pGameObject들의 Update를 실행하는 구문이다.
			// 오브젝트 매니저를 통해 맵 컨테이너에 담겨져 있는 실 사용 오브젝트들의 Update를 해준다.
			// 이 때, 오브젝트들의 List를 가지고 있는 Layer Class에 접근하여 Update_Layer 함수를 실행한다.
			iExitCode = Pair.second->Update_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

_int CObject_Manager::LateUpdate_Object_Manager(const _float & fTimeDelta)
{
	// 위랑 동일한 기능인데 끝나고 한번 더부르는 거쥬?

 	if (nullptr == m_pmapObject)
		return -1;

	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxNumScene; ++i)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			iExitCode = Pair.second->LateUpdate_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

CLayer * CObject_Manager::FindObjectLayer(const _uint & iSceneIdx, const _tchar * pLayerTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	auto	iter = find_if(m_pmapObject[iSceneIdx].begin(), m_pmapObject[iSceneIdx].end(),
		[&pLayerTag](auto& Pair)->bool {return !lstrcmp(pLayerTag, Pair.first) ? true : false; });

	if (iter == m_pmapObject[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

CGameObject * CObject_Manager::Find_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype)
		return nullptr;

	// 찾아서 iter에 정보를 넣어주세용.
	auto	iter = find_if(m_pmapPrototype[iSceneIdx].begin(), m_pmapPrototype[iSceneIdx].end(), CFinder_Tag(pProtoTag));
	//auto	iter = find_if(m_pMapPrototype[iSceneIdx].begin(), m_pMapPrototype[iSceneIdx].end(), [&](MAPPROTOTYPE::value_type Pair)->bool {if (0 == lstrcmp(pProtoTag, Pair.first)) return true; return false; });

	// 없으면 nullpter을 return 조집니다.
	if (iter == m_pmapPrototype[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(const _uint & iSceneIdx, const _tchar * pLayerTag)
{
	// 실 사용 객체들을 보관하는 컨테이너를 뒤지는 곳이다.

	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	auto	iter = find_if(m_pmapObject[iSceneIdx].begin(), m_pmapObject[iSceneIdx].end(), CFinder_Tag(pLayerTag));

	if (iter == m_pmapObject[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapPrototype[i])
		{
			_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
			Safe_Delete_Array(pTempArray);
			Safe_Release(Pair.second);
		}

		m_pmapPrototype[i].clear();
	}
	Safe_Delete_Array(m_pmapPrototype);

	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
			Safe_Delete_Array(pTempArray);
			Safe_Release(Pair.second);
		}

		m_pmapObject[i].clear();
	}
	Safe_Delete_Array(m_pmapObject);
}
