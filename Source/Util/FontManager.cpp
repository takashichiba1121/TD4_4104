#include "FontManager.h"

std::unordered_map<std::string,int32_t> FontManager::fonts_;

void FontManager::CreateFontHandle(const TCHAR* name,int32_t size,int32_t tick,std::string tag)
{
	int32_t temp = CreateFontToHandle(name,size,tick);

	fonts_[ tag ] = temp;

}

int32_t FontManager::GetFontHandle(std::string tag)
{
	return fonts_[tag];
}

void FontManager::Finalize()
{
	for ( auto& itr : fonts_ )
	{
		DeleteFontToHandle(itr.second);
	}
}
