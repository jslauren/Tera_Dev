#pragma once

// For.Singleton
#define _NO_COPY(CLASSNAME)									\
	private:												\
	CLASSNAME(const CLASSNAME&);							\
	CLASSNAME& operator = (const CLASSNAME&);		

#define _DECLARE_SINGLETON(CLASSNAME)						\
	_NO_COPY(CLASSNAME)										\
	private:												\
	static CLASSNAME*	m_pInstance;						\
	public:													\
	static CLASSNAME*	GetInstance( void );				\
	static unsigned long DestroyInstance( void );					

#define _IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
		if(NULL == m_pInstance) {							\
			m_pInstance = new CLASSNAME;					\
		}													\
		return m_pInstance;									\
	}														\
	unsigned long CLASSNAME::DestroyInstance( void ) {		\
		unsigned long dwRefCnt = 0;							\
		if(NULL != m_pInstance)	{							\
			dwRefCnt = m_pInstance->Release();				\
			if(0 == dwRefCnt)								\
				m_pInstance = NULL;							\
		}													\
		return dwRefCnt;									\
	}

// For.MessageBox
#define _MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("Error"), MB_OK);

// For.NameSpace
#define _BEGIN(NAME)	namespace NAME {
#define _END			}
#define _USING(NAME)	using namespace NAME;

// For.Export
#ifdef ENGINE_EXPORTS
#define _DLL_EXPORTS _declspec(dllexport)
#else
#define _DLL_EXPORTS _declspec(dllimport)
#endif
