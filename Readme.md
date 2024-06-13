# Hollow Engine

## Guide-Book

- P.S: Code base is designed to run on Visual Studio 2022 or later versions with CMake features.

1. Download latest Vulkan SDK from [Download Link from LunarG](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe).
1.1 Make sure to check the boxes <b>Shader Toolchain Debug Symbols - 64bit</b>, <b>SDL2 libraries and headers</b> and <b>Vulkan Memory Allocator header</b>

2. Run the <b>GenerateAssociation.bat</b> file to associate the <b>.hproject</b> files with the <b>Hollow Engine</b> editor. <b>(W.I.P)</b>
3. Run the <b>GenerateProject.bat</b> file to generate the Visual Studio solution. <b>(W.I.P)</b>

## Features
- [x] Graphics Abstraction Layer
- [x] Vulkan Renderer
- [ ] DirectX-12 Renderer
- [x] Win32
- [x] GLFW
- [ ] Linux
- [ ] MacOS
- [x] ResourceSystem
- [ ] Editor

## Dependencies
- [x] Vulkan SDK
- [ ] DirectX 12 SDK
- [x] Assimp
- [x] ImGui
- [x] EnTT
- [ ] PhysX
- [ ] FMod
- [x] STBI
- [ ] Nlohmann JSON

## What have I done so far:
- Independent Graphics API. Be able to support multiple APIs. Modifications can be done on only higher level to swap between apis.
- Independent Platform. Be able to support multiple operating system libraries.
- Resource Management for RenderTarget and Mesh.
- Basic Assimp implementation. After shading is done,

## Tests
- [x] Instancing
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/2fe2fa5a-80fe-459a-a826-2bd75ec1bef2)
- [x] Mesh Importing
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/478829b7-6e18-4c07-a822-1f36f174eaf2)

- [x] Shading & Basic Illumination
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/38944789-14d4-43e5-9b63-c78f8e6517eb)

- [ ] Sponza Import
- [ ] PBR Material
- [ ] Basic Imgui Renderer
