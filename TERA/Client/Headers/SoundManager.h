#pragma once
#include "Base.h"
#include "fmod.h"

_BEGIN(Client)

class CSoundManager final : public CBase
{
	_DECLARE_SINGLETON(CSoundManager)

public:
	enum Channel_ID 
	{
		CH_BGM, CH_ATTACK, CH_SKILL, CH_PLAYER_VOICE, CH_TOWN_NPC, CH_END
	};

private:
	explicit CSoundManager();
	virtual ~CSoundManager() = default;

public:
	HRESULT		ReadySound();
	void		UpdateSound();
	void		Play_SoundChannel(const string& pSoundKey, Channel_ID eID, bool bLoop);
	void		Play_BGM(const string& pSoundKey);
	void		Stop_Sound(Channel_ID eID);
	void		Stop_AllSound();

private:
	HRESULT		LoadSoundFile();

private:
	map<string, FMOD_SOUND*>		m_mapSound;					// FMOD_SOUND: 사운드 파일에 대한 정보를 갖는 구조체.
	FMOD_CHANNEL*					m_pChannelArr[CH_END];		// FMOD_CHANNEL: 사운드를 재생하고 관리하는 역할. ex) 볼륨 조절 등.
	FMOD_SYSTEM*					m_pSystem;					// FMOD_SYSTEM: FMOD_SOUND과 FMOD_CHANNEL을 총괄 관리하는 객체.

public:
	virtual void	Free();

};

_END