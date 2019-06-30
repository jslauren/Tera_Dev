#pragma once
#include "Base.h"

_BEGIN(Client)

class CUI_DamageTexture;
class CUI_DamageFont_Manager final : public CBase
{
	_DECLARE_SINGLETON(CUI_DamageFont_Manager)
private:
	explicit CUI_DamageFont_Manager();
	virtual ~CUI_DamageFont_Manager() = default;

public:
	HRESULT	Create_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3& vPos, const float& fDamageValue);
	void	Update_DamageFont(const float& fTimeDelta);
	void	Render_DamageFont();

private:
	list<CUI_DamageTexture*>	m_DamageFontList;

public:
	virtual void Free();

};

_END