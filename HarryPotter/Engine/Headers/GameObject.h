#pragma once
#include "Base.h"

_BEGIN(Engine)

class _DLL_EXPORTS CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);	// 복사생성자 재정의
	virtual ~CGameObject() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	virtual CGameObject* Clone() = 0;
	virtual void Free();
};

_END