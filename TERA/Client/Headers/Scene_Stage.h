#pragma once
#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CPlayer;
class CScene_Stage final : public CScene
{
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual HRESULT	Init_Setting();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();

private:
	HRESULT	Ready_TerrainLoad();
	HRESULT	Ready_MeshLoad();
	HRESULT Ready_LightInfo();
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Weapon(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

private:
	CPlayer*								m_pPlayer = nullptr;

private:
	map<int, TERRAINDATA*>					m_MapTerrainData;
	map<wstring, vector<OBJECTMESHDATA>>	m_MapMeshData;
	_uint									m_iCurrentCellIdx = 0;
	_float									m_fDetail = 1.f;
	_tchar									m_szPlayerPos[128] = L"";

public:
	static CScene_Stage*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free();
};

_END