#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <functional>

class FilePicker
{
public:
	using OnPicked = std::function<void(const std::filesystem::path&)>;
	struct Settings {
		bool foldersOnly{ false };
		std::vector<std::string> extensionsFilter;
	};
public:
	FilePicker();
	~FilePicker() = default;

	// Call each frame to refresh and display the explorer
	void Update();

	// Set a texture to be used as icon near folders
	void SetFolderIcon(const sf::Texture& icon) { mFolderIcon = icon; }
	// Set a texture to be used as icon near files with matching extension
	void SetFileIcon(const std::string& extension, const sf::Texture& icon) { mFileIconPerExtension[extension] = icon; }
	// Set a texture to be used as icon near file if no specific icon exists for its extension
	void SetDefaultFileIcon(const sf::Texture& icon) { mDefaultFileIcon = icon; }
	
	// Starts picking a folder
	void PickFolder(const std::filesystem::path& startingPath, const OnPicked& onPicked);
	// Starts picking a file
	void PickFile(const std::filesystem::path& startingPath, const OnPicked& onPicked, const std::vector<std::string>& extensionFilters = std::vector<std::string>());
	void StartPicking(const std::filesystem::path& startingPath, const OnPicked& onPicked, const Settings& settings = Settings());

	inline void Show() { mVisible = true; }
	inline void Hide() { mVisible = false; }
	inline bool IsVisible() const { return mVisible; }

private:
	const sf::Texture& GetFileIcon(const std::string& extension) const;
	void SetPath(const std::filesystem::path& path);

private:
	sf::Texture mFolderIcon;
	sf::Texture mDefaultFileIcon;
	std::unordered_map<std::string, sf::Texture> mFileIconPerExtension;
	std::filesystem::path mPath;
	OnPicked mOnPicked;
	Settings mSettings;
	bool mVisible{ false };
};