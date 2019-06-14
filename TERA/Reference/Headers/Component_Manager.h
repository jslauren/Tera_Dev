#pragma once
#include "Base.h"
#include "Shader.h"
#include "Frustum.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "Mesh_Dynamic_Bone.h"
#include "Mesh_Dynamic_Parts.h"
#include "Buffer_RcCol.h"
#include "Buffer_RcTex.h"
#include "Buffer_TriCol.h"
#include "Buffer_Terrain.h"
#include "Buffer_CubeTex.h"
#include "Buffer_ScreenTex.h"

// �پ��� ������Ʈ Ŭ�������� �θ� �Ǵ� Ŭ����.
// ������Ʈ�� �����Ѵ�.
// ������Ʈ�� ������ ������ �پ� (��������, �ؽ���, �޽�, ����, ��ȯ, ������)

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
	CComponent* Clone_Component(const _uint& iSceneIdx, const _tchar* pComponentTag, void* pArg = nullptr);
public:
	CComponent*	Find_Component_Prototype(const _uint& iSceneIdx, const _tchar* pComponentTag);
private:
	// ���� ��ü�� Prototype�� �̸��� �ٿ���� �� �򰥸��� ������...?
	// m_pMapComponentPrototype �̷� ������...
	map<const _tchar*, CComponent*>*		m_pmapComponentPrototype = nullptr; // ������ü�� ��Ƴ��´�.
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint		m_iMaxNumScene = 0;
public:
	virtual void Free();
};

_END