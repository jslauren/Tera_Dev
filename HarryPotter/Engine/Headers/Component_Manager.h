#pragma once
// 다양한 컴포넌트 클래스들의 부모가 되는 클래스.

#include "Base.h"
#include "Buffer_TriCol.h"
#include "Transform.h"
#include "Renderer.h"

// 컴포넌트를 보관한다.
// 컴포넌트의 종류가 굉장히 다양하다. (정점버퍼, 텍스쳐, 메시, 사운드, 변환, 렌더러)

_BEGIN(Engine)

class _DLL_EXPORTS CComponent_Manager final : public CBase
{
	_DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT		Reserve_Component_Manager(const _uint& iMaxNumScene);
	HRESULT		Add_Component_Prototype(const _uint& iSceneIdx, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Clone_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
private:
	map<const _tchar*, CComponent*>*		m_pmapComponent = nullptr; // 원형객체를 모아놓는다.
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint	m_iMaxNumScene = 0;
private:
	CComponent* Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
public:
	virtual void Free();
};

_END