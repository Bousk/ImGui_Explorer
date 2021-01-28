#include <FilePicker.hpp>

#include <imgui/SFML.h>

#include <imgui.h>

#include <sstream>

FilePicker::FilePicker()
{
	mPath = std::filesystem::absolute(std::filesystem::current_path());
}
void FilePicker::Update()
{
	namespace ImGui = ::ImGui;

	if (IsVisible())
	{
		if (ImGui::Begin("FilePicker", &mVisible))
		{
			// Show the current path with clickable button for each folder to travel through
			ImGui::Text("Current path : ");
			std::filesystem::path fullpath;
			for (const std::filesystem::path& path : mPath)
			{
				fullpath /= path;
				ImGui::SameLine();
				if (ImGui::Button(path.string().c_str()))
				{
					SetPath(fullpath);
					break;
				}
			}

			auto ButtonWithIcon = [](const std::string& name, const sf::Texture& icon)
			{
				ImGui::PushID(name.c_str());
				bool clicked = false;
				if (icon.getSize().x && icon.getSize().y)
				{
					const ImTextureID imTex = ImGuiSFML::GLHandleToImTexture(icon.getNativeHandle());
					clicked |= ImGui::ImageButton(imTex, ImVec2(16, 16));
					ImGui::SameLine();
				}
				clicked |= ImGui::Button(name.c_str());
				ImGui::PopID();
				return clicked;
			};
			auto ButtonFile = [&, this](const std::string& name) { return ButtonWithIcon(name, GetFileIcon(std::filesystem::path(name).extension().string())); };
			auto ButtonFolder = [&, this](const std::string& name) { return ButtonWithIcon(name, mFolderIcon); };
			auto IsMatchingFilter = [this](const std::string& extension) { return mSettings.extensionsFilter.empty() || std::any_of(mSettings.extensionsFilter.begin(), mSettings.extensionsFilter.end(), [&](const std::string& ext) { return ext == extension; }); };

			if (mPath.has_parent_path())
			{
				const std::filesystem::path parent = mPath.parent_path();
				if (ButtonFolder(".."))
				{
					SetPath(parent);
				}
			}

			while (!std::filesystem::exists(mPath))
				mPath = mPath.parent_path();

			for (const auto& entry : std::filesystem::directory_iterator(mPath))
			{
				const std::filesystem::path& path = entry.path();
				const std::string entryName = std::filesystem::relative(path, mPath).string();
				if (entry.is_directory())
				{
					if (ButtonFolder(entryName))
					{
						SetPath(path);
						break;
					}
				}
				else if (entry.is_regular_file())
				{
					if (!mSettings.foldersOnly)
					{
						if (IsMatchingFilter(path.filename().extension().string()))
						{
							if (ButtonFile(entryName.c_str()))
							{
								if (mOnPicked)
									mOnPicked(path);
								Hide();
							}
						}
					}
				}
				else
				{
					ImGui::Text(entryName.c_str());
				}
			}
			if (mSettings.foldersOnly)
			{
				if (ImGui::Button("Select this folder"))
				{
					if (mOnPicked)
						mOnPicked(mPath);
					Hide();
				}
			}
		}
		ImGui::End();
	}
}

void FilePicker::SetPath(const std::filesystem::path& path)
{
	mPath = path;
	// Remove ending / if any
	if (mPath.wstring().back() == std::filesystem::path::preferred_separator)
		mPath = mPath.parent_path();
}

const sf::Texture& FilePicker::GetFileIcon(const std::string& extension) const
{
	const auto it = mFileIconPerExtension.find(extension);
	return it != mFileIconPerExtension.cend() ? it->second : mDefaultFileIcon;
}

void FilePicker::PickFolder(const std::filesystem::path& startingPath, const OnPicked& onPicked)
{
	Settings settings;
	settings.foldersOnly = true;
	StartPicking(startingPath, onPicked, settings);
}
void FilePicker::PickFile(const std::filesystem::path& startingPath, const OnPicked& onPicked, const std::vector<std::string>& extensionFilters/* = std::vector<std::string>()*/)
{
	Settings settings;
	settings.extensionsFilter = extensionFilters;
	StartPicking(startingPath, onPicked, settings);
}
void FilePicker::StartPicking(const std::filesystem::path& startingPath, const OnPicked& onPicked, const Settings& settings/* = Settings()*/)
{
	SetPath(startingPath);
	Show();
	mOnPicked = onPicked;
	mSettings = settings;
}