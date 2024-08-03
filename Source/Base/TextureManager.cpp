#include "TextureManager.h"
#include<DxlibInclude.h>

uint32_t TextureManager::LoadTexture(const std::string& path_)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto textureItr = find_if(sTextureDates.begin(), sTextureDates.end(), [&](std::pair<const std::string, TextureManager::Tex>& texture)
		{
			return texture.second.path == path_;
		});

	if (textureItr == sTextureDates.end())
	{
		Tex data;

		data.path = path_;
		data.id = DxLib::LoadGraph(path_.c_str());

		sTextureDates[path_] = data;

		return data.id;
	}
	else
	{

		return textureItr->second.id;
	}

	return 0;
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;

	return &instance;
}

uint32_t TextureManager::Load(const std::string& path_)
{
	return GetInstance()->LoadTexture(path_);
}

