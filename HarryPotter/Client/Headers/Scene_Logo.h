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
	// 현재 씬의 Update호출 이후 최종적으로 한번 더 호출되는 Update
	// 씬안에서 객체들의 업데이트 호출. 모든 객체들의 업데이트를 호출한 후, 다시 한번 객체들의 업데이트를 호출.
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();
private:
	// 이 씬에서 사용할 객체들의 Initialize 작업을 위한 멤버함수 선언이다.
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();
};

_END