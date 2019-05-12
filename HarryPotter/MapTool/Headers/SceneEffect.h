#pragma once
#include "SCene.h"

class CSceneEffect final : public CScene
{
public:
	explicit CSceneEffect(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual ~CSceneEffect() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta = 1.f) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta = 1.f) override;
	virtual HRESULT Render_Scene() override;

private:
	//������ ����ϴ� ������Ʈ ������Ÿ�Ե��� �غ�
	HRESULT ReadyGameObject();

public:
	static CSceneEffect* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual void Free();

};

