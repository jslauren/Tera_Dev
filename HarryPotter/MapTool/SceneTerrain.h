#pragma once
#include "Scene.h"
#include "ViewManager.h"
#include "DataManager.h"

class CSceneTerrain final : public CScene
{
public:
	enum CAMERAMODE { CAM_EDIT, CAM_VIEW, CAM_END };
private:
	explicit CSceneTerrain(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~CSceneTerrain() = default;

//public:
//	virtual HRESULT ReadyScene() override;
//	virtual _int	UpdateScene(const _float& fTimeDelta = 1.f) override;
//	virtual _int	LateUpdateScene(const _float& fTimeDelta = 1.f) override;
//	virtual HRESULT RenderScene() override;

public:
	void	SetMapSize(_int iCountX, _int iCountZ);

private:
	HRESULT ReadyGameObject(const _tchar* pLayerTag);

private:
	CViewManager* m_pViewManager = nullptr;
	CDataManager* m_pDataManager = nullptr;

public:
	static CSceneTerrain* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual void Free();
};

