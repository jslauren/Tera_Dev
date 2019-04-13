#pragma once
// �پ��� ������Ʈ Ŭ�������� �θ� �Ǵ� Ŭ����.

#include "Base.h"
#include "Buffer_TriCol.h"
#include "Transform.h"
#include "Renderer.h"

// ������Ʈ�� �����Ѵ�.
// ������Ʈ�� ������ ������ �پ��ϴ�. (��������, �ؽ���, �޽�, ����, ��ȯ, ������)

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
	map<const _tchar*, CComponent*>*		m_pmapComponent = nullptr; // ������ü�� ��Ƴ��´�.
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint	m_iMaxNumScene = 0;
private:
	CComponent* Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
public:
	virtual void Free();
};

_END