#pragma once
#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	// ���� ���� Updateȣ�� ���� ���������� �ѹ� �� ȣ��Ǵ� Update
	// ���ȿ��� ��ü���� ������Ʈ ȣ��. ��� ��ü���� ������Ʈ�� ȣ���� ��, �ٽ� �ѹ� ��ü���� ������Ʈ�� ȣ��.
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();
private:
	HRESULT Ready_GameObject_Prototype();
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

_END