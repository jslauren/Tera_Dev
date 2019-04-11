#pragma once

namespace Engine
{
	// Functor
	// 함수객체 : 객체를 함수처럼 사용할 수 있게 만들어진 클래스

	class CFinder_Tag
	{
	public:
		// 함수 객체에 인자를 외부 인자를 받기 위해 생성자를 선언 및 정의 해준다.
		explicit CFinder_Tag(const _tchar* pTag) : m_pTag(pTag) {}
		CFinder_Tag() = default;
	public:
		template<typename T>
		bool operator () (T Pair)
		{
			return !lstrcmp(Pair.first, m_pTag);
		}
	private:
		const _tchar*		m_pTag = nullptr;
	};
}