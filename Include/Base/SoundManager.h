#include<map>
#include<string>

class SoundManager
{
private:

	struct Sound
	{
		std::string path;
		int32_t id;
	};

	std::map<std::string,Sound> soundDates_;

public:

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	uint32_t LoadSound(const std::string& path_);

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static SoundManager* GetInstance();

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& path_);

private:

	~SoundManager() = default;
	SoundManager() = default;
};

