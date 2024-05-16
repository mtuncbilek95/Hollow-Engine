# Hollow Engine

## Guide-Book

- P.S: Code base is designed to run on Visual Studio 2022 or later versions with CMake features.

1. Download latest Vulkan SDK from [Download Link from LunarG](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe).
1.1 Make sure to check the boxes <b>Shader Toolchain Debug Symbols - 64bit</b>, <b>SDL2 libraries and headers</b> and <b>Vulkan Memory Allocator header</b>

2. Run the <b>GenerateAssociation.bat</b> file to associate the <b>.hproject</b> files with the <b>Hollow Engine</b> editor. <b>(W.I.P)</b>
3. Run the <b>GenerateProject.bat</b> file to generate the Visual Studio solution. <b>(W.I.P)</b>

## Features
- [x] Vulkan Renderer
- [ ] DirectX-12 Renderer
- [x] Win32
- [ ] Linux
- [ ] MacOS
- [x] ResourceSystem <b>(W.I.P)</b>
- [ ] Editor <b>(W.I.P)</b>

## Dependencies
- [x] Vulkan SDK
- [ ] DirectX 12 SDK
- [x] Assimp
- [x] ImGui
- [ ] EnTT
- [ ] PhysX
- [ ] FMod
- [x] STBI
- [x] Nlohmann JSON

## Tests
- [x] Runtime Test
- [x] Resource Management for Shader
- [ ] Resource Management for Texture <b>(W.I.P)</b>
- [ ] Resource Management for Mesh
