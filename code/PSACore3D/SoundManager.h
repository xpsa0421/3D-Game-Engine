#pragma once
#include "Sound.h"
#include <io.h>
#define S_SoundManager SoundManager::GetInstance()

class SoundManager : public Singleton<SoundManager>
{
private:
	friend class Singleton<SoundManager>;
	FMOD::System* _system = nullptr;
private:
	std::list<W_STR>			_filePaths;
	std::map<W_STR, Sound*>	_sounds;
public:
	void	LoadDir(W_STR path);
	void	LoadAll(W_STR path);
	Sound*	LoadSound(W_STR filePath);
	T_STR	GetFileName(W_STR filePath);
	Sound*	GetSoundPtr(W_STR fileName);
public:
	bool	Init();
	bool	Frame();
	bool	Release();
private:
	SoundManager();
public:
	~SoundManager();
};

