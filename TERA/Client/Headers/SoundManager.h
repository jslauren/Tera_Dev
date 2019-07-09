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
	map<string, FMOD_SOUND*>		m_mapSound;					// FMOD_SOUND: ���� ���Ͽ� ���� ������ ���� ����ü.
	FMOD_CHANNEL*					m_pChannelArr[CH_END];		// FMOD_CHANNEL: ���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_SYSTEM*					m_pSystem;					// FMOD_SYSTEM: FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.

public:
	virtual void	Free();

};

_END