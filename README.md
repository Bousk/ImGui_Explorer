# ImGui_Explorer
A simple file explorer / file picker / folder picker using ImGui &amp; std::filesystem
Works with ImGuiSFML to handle icons, which provides a nice SFML, see here https://github.com/Bousk/ImGuiSFML
If you're not using it already and don't intend to, you'll need to provide a replacement for GLHandleToImTexture method and change the sf::Texture (SFML texture) to yours

# How to use

    // Create an instance of the explorer
    FilePicker mImguiFilePicker;

    // You can then assign icons to file extensions for better readibility when in use
    sf::Texture iniTex;
    iniTex.loadFromFile("Icons/Ini.png");
    mImguiFilePicker.SetFileIcon(".ini", iniTex);
    // You can also assign icon to folders
    sf::Texture folderTex;
    folderTex.loadFromFile("Icons/Folder.png");
    mImguiFilePicker.SetFolderIcon(folderTex);
    // You can assign default icon to files whose extensions don't have specific icon
    sf::Texture defaultFileTex;
    defaultFileTex.loadFromFile("Icons/File.png");
    mImguiFilePicker.SetDefaultFileIcon(defaultFileTex);
    
    // Then call regularly, to update the explorer/picker
    mImguiFilePicker.Update();
        
    // To start using the picking capability    
    // To pick a file with no extension filter
    if (ImGui::Button("Open file picker"))
        mImguiFilePicker.PickFile(std::filesystem::current_path(), [&](const std::filesystem::path& selectedPath) {});
    // To pick a file with specific filter
    if (ImGui::Button("Open file picker"))
        mImguiFilePicker.PickFile(std::filesystem::current_path(), [&](const std::filesystem::path& selectedPath) {}, std::vector<std::string>(1, ".ini"));
    // To pick a folder
    if (ImGui::Button("Open folder picker"))
        mImguiFilePicker.PickFolder(std::filesystem::current_path(), [&](const std::filesystem::path& selectedPath) {});
