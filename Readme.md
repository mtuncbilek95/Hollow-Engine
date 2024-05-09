# Hollow Engine

## Guide-Book

- P.S: Code base is designed to run on Visual Studio 2022 or later versions with CMake features.

1. Download latest Vulkan SDK from [Download Link from LunarG](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe).
1.1 Make sure to check the boxes <b>Shader Toolchain Debug Symbols - 64bit</b>, <b>SDL2 libraries and headers</b> and <b>Vulkan Memory Allocator header</b>

2. Run the <b>GenerateAssociation.bat</b> file to associate the <b>.hproject</b> files with the <b>Hollow Engine</b> editor. <b>(W.I.P)</b>
3. Run the <b>GenerateProject.bat</b> file to generate the Visual Studio solution. <b>(W.I.P)</b>

## Features
- [ ] Vulkan Renderer
- [ ] DirectX-12 Renderer
- [x] Win32
- [ ] Linux
- [ ] MacOS
- [ ] ResourceSystem <b>(W.I.P)</b>
- [ ] Editor <b>(W.I.P)</b>

## Dependencies
- [ ] Vulkan SDK
- [ ] DirectX 12 SDK
- [ ] Assimp
- [ ] ImGui <b>(W.I.P)</b>
- [ ] PhysX
- [ ] FMod
- [ ] STBI
- [ ] Nlohmann JSON

## Tests
- [ ] Hello Triangle
- [ ] Rotating Square in 2D
- [ ] Rotating Textured Cube
- [ ] Instanced Cube with Multiple Positions
- [ ] Json Serializer Implementation
- [ ] Mesh_Import