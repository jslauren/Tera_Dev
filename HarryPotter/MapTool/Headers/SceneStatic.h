#pragma once
#include "SCene.h"

class CSceneStatic final : public CScene
{
private:
	explicit CSceneStatic(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual ~CSceneStatic() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta = 1.f) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta = 1.f) override;
	virtual HRESULT Render_Scene() override;

private:
	//������ ����ϴ� ������Ʈ ������Ÿ�Ե��� �غ�
	HRESULT ReadyGameObject();

public:
	static CSceneStatic* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual void Free();

};

