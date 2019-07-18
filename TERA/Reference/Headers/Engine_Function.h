#pragma once

namespace Engine
{
	static void Lerp(float* pOut, float fSour, float fDest, float fTimeDelta)
	{
		// 선형 보간.
		// 두 값의 자연스러운 이동을 위한 함수.
		// 중간 값을 찾아준다?
		// Feat.선호
		*pOut = ((fDest - fSour) * fTimeDelta) + fSour;
	};

	struct DeleteMap
	{
		template<typename T>
		void operator()(T& Temp)
		{
			if (Temp.second)
			{
				delete Temp.second;
				Temp.second = nullptr;
			}
		}
	};

	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;
		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->AddRef();
		}

		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;
		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
		return dwRefCnt;
	}
}