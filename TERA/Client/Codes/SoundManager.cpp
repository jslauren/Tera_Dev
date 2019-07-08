#include "stdafx.h"
#include "..\Headers\SoundManager.h"
#include <io.h>

_USING(Client)

_IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
{
}

HRESULT CSoundManager::ReadySound()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CH_END, FMOD_INIT_NORMAL, nullptr);

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return NOERROR;
}

void CSoundManager::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::Play_SoundChannel(const string& pSoundKey, Channel_ID eID, bool bLoop)
{
	auto iter_find = m_mapSound.find(pSoundKey);

	if (m_mapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, false, &m_pChannelArr[eID]);

	if (true == bLoop)
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);

	UpdateSound();
}

void CSoundManager::Play_BGM(const string& pSoundKey)
{
	auto iter_find = m_mapSound.find(pSoundKey);

	if (m_mapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, false, &m_pChannelArr[CH_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[CH_BGM], FMOD_LOOP_NORMAL);

	UpdateSound();
}

void CSoundManager::Stop_Sound(Channel_ID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
	UpdateSound();
}

void CSoundManager::Stop_AllSound()
{
	for (int i = 0; i < CH_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);

	UpdateSound();
}

HRESULT CSoundManager::LoadSoundFile()
{
	_finddata_t FindData;
	int handle = _findfirst("../Bin/Resources/Sound/*.*", &FindData);

	if (0 == handle)
		return E_FAIL;

	char szFullPath[256] = "";
	int iContinue = 0;

	while (-1 != iContinue)
	{
		strcpy(szFullPath, "../Bin/Resources/Sound/");
		strcat(szFullPath, FindData.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, nullptr, &pSound);

		if (FMOD_OK == eResult)
		{
			string strSoundKey = string(FindData.name);

			auto iter_find = m_mapSound.find(strSoundKey);

			if (m_mapSound.end() != iter_find)
			{
				FMOD_Sound_Release(pSound);
				continue;
			}
			m_mapSound[strSoundKey] = pSound;
		}
		iContinue = _findnext(handle, &FindData);
	}
	FMOD_System_Update(m_pSystem);

	return NOERROR;
}

void CSoundManager::Free()
{
	for (auto& Sound : m_mapSound)
		FMOD_Sound_Release(Sound.second);

	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
