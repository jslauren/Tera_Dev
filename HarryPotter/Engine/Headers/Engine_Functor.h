#pragma once

namespace Engine
{
	// Functor
	// 함수객체 : 객체를 함수처럼 사용할 수 있게 만들어진 클래스

	class CFinder_Tag
	{
	public:
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