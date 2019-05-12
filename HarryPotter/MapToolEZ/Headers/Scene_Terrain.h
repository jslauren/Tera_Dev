#pragma once
#include "Tool_Defines.h"
#include "Scene.h"

_BEGIN(MapTool)

class CScene_Terrain final : public CScene
{
private:
	explicit CScene_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Terrain() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();
private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);

public:
	static CScene_Terrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free();

};

_END