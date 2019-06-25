#pragma once
#include "Base.h"
#include "Engine_Defines.h"

// 현재 생성된 씬을 보유하고 있다.
// 씬의 핵심적인 함수를 호출하여 내 게임의 전체적인 흐름을 관리한다.
// 그냥 씬 매니저 그 자체.. 인줄 알았는데.. 그냥 복합적으로 믹스되서,
// 매니지먼트... 인것같다.

_BEGIN(Engine)

class CScene;
class CRenderer;
class _DLL_EXPORTS CManagement final : public CBase
{
	_DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement() = default;
public:
	_uint			Get_CurrentScene() { return m_iCurrentSceneIndex; }
	_uint			Get_PreScene() { return m_iPreSceneIndex; }
public:
	HRESULT			SetUp_CurrentScene(CScene* pScene, const _uint& iCurrentSceneIndex = 0);
	void			Set_CurrentScene(_uint _iCurentSceneIndex) { m_iCurrentSceneIndex = _iCurentSceneIndex; }
public:
	HRESULT			Ready_Management(const _uint& iMaxNumScene);
	_int			Update_Management(const _float& fTimeDelta);
	HRESULT			Render_Management();
	static HRESULT	Release_Engine();
private:
	CScene*			m_pCurrentScene = nullptr;
	_uint			m_iCurrentSceneIndex = 1;
	_uint			m_iPreSceneIndex = 1;
public:
	virtual void	Free();
};

_END