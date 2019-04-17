#pragma once

#include "Base.h"

// 다양한 컴포넌트 클래스들의 부모가 되는 클래스.
// 이제 객체들이 이 클래스의 자식클래스들인 렌더러나 버퍼, 텍스쳐, 트랜스폼 객체(클래스)들을,
// 멤버함수로 가지며 해당 기능들을 사용할 수 있게 된다.

_BEGIN(Engine)

class _DLL_EXPORTS CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	virtual HRESULT Ready_Component();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	// 해당 컴포넌트가 복사된 놈인지, 원본인지 확인하는 멤버 불 변수
	_bool					m_isClone = false;
public:
	virtual CComponent* Clone() = 0;
	virtual void Free();
};

_END