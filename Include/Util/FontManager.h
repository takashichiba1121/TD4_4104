#pragma once
#include <unordered_map>
#include <stdint.h>
#include <string>
#include <DxlibInclude.h>

class FontManager
{

private:
	static std::unordered_map<std::string,int32_t> fonts_;

public:
	static void CreateFontHandle(const TCHAR* name,int32_t size,int32_t tick,std::string tag);
	static int32_t GetFontHandle(std::string tag);
	static void Finalize();
};

