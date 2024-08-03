#include "SoundManager.h"
#include<DxlibInclude.h>

uint32_t SoundManager::LoadSound(const std::string& path_)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto textureItr = find_if(soundDates_.begin(),soundDates_.end(), [&](std::pair<const std::string,SoundManager::Sound>& texture)
		{
			return texture.second.path == path_;
		});

	if (textureItr == soundDates_.end())
	{
		Sound data;

		data.path = path_;
		data.id = DxLib::LoadGraph(path_.c_str());

		soundDates_[path_] = data;

		return data.id;
	}
	else
	{
		return textureItr->second.id;
	}

	return 0;
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;

	return &instance;
}

uint32_t SoundManager::Load(const std::string& path_)
{
	return GetInstance()->LoadSound(path_);
}

