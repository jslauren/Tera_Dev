#pragma once
#include "Base.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"

#include "Buffer_RcCol.h"
#include "Buffer_RcTex.h"
#include "Buffer_TriCol.h"
#include "Buffer_Terrain.h"
#include "Buffer_CubeTex.h"
#include "Buffer_ScreenTex.h"

// 다양한 컴포넌트 클래스들의 부모가 되는 클래스.
// 컴포넌트를 보관한다.
// 컴포넌트의 종류가 굉장히 다양 (정점버퍼, 텍스쳐, 메시, 사운드, 변환, 렌더러)

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
	HRESULT		Clear_Component_Prototype(const _uint& iSceneIdx);
	CComponent* Clone_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
private:
	// 원형 객체면 Prototype을 이름에 붙여줘야 안 헷갈리지 않을까...?
	// m_pMapComponentPrototype 이런 식으로...
	map<const _tchar*, CComponent*>*		m_pmapComponent = nullptr; // 원형객체를 모아놓는다.
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint		m_iMaxNumScene = 0;
private:
	CComponent*	Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
public:
	virtual void Free();
};

_END