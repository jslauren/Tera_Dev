#pragma once

namespace Engine
{
	// Functor
	// �Լ���ü : ��ü�� �Լ�ó�� ����� �� �ְ� ������� Ŭ����

	class CFinder_Tag
	{
	public:
		// �Լ� ��ü�� ���ڸ� �ܺ� ���ڸ� �ޱ� ���� �����ڸ� ���� �� ���� ���ش�.
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