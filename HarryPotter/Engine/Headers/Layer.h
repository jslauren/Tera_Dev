#pragma once

#include "Base.h"
// ������Ʈ�� �����̳�(List)�� ������ �ִ� Ŭ���� CLayer Ŭ���� �ǽðڴ�.
_BEGIN(Engine)

class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	HRESULT Ready_Layer();
public:
	// �ش� �����̳ʸ� ����ϴ� �Լ����� ��� ������ ���Ҵ�.
	HRESULT Add_ObjectToLayer(CGameObject* pGameObject);
	_int	Update_Layer(const _float& fTimeDelta);
	_int	LateUpdate_Layer(const _float& fTimeDelta);
private:
	// ��ü���� ��Ƴ��� ������Ʈ ����Ʈ �����̳�.
	// (�ǻ�� ��ü, ������ü) �� �߿� �ϳ��� �ƴ� �� ���� ��� ������ �ִ�
	// ������Ʈ Ŭ������ ����Ʈ�ȿ� �־� ������ �ִ�.
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
	virtual void Free();
};

_END