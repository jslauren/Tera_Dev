#pragma once
#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_Dragon final : public CScene
{
private:
	explicit CScene_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Dragon() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();
private:
	HRESULT	Ready_TerrainLoad();
	HRESULT	Ready_MeshLoad();
	HRESULT Ready_LightInfo();
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_Loading(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Weapon(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
private:
	void	SetCutSceneEvent();
private:
	_float	m_fDetail = 1.f;
	_bool	m_bIsSceneFirstPlay = true;
private:
	map<int, TERRAINDATA*>					m_MapTerrainData;
	map<wstring, vector<OBJECTMESHDATA>>	m_MapMeshData;
public:
	static CScene_Dragon*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free();
};

_END