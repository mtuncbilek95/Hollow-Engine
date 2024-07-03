# Hollow Engine

## Guide-Book

### IMPORTANT NOTE
Since the design needs manual memory management at the moment, if you want to change the window size or you want to import another mesh, please make sure that you 
change the mDeviceMemory and the mHostMemory. Otherwise, the program will create artifacts. My advice is to define memory as 1024MB for each of them.
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/c279df00-cac8-46d5-aab2-66c22dea34bb)

### Introduction
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
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/4151e3d1-92ac-40ba-8aaa-e155ca1a06c5)
- [x] Sponza Import
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/571c0793-efb3-4b05-b82d-4d96932247a2)
- [x] Sponza Diffuse Light
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/88eb8afd-6a66-495c-89e5-1c19686b94fc)
- [x] PBR Material without Skybox
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/2a13819d-e582-479a-ba5d-273d25846c0a)
- [x] PRB Material with Skybox
![image](https://github.com/mtuncbilek95/Hollow-Engine/assets/50660242/22da7fb1-0bcd-4ead-bc69-44a2ae1fa3c6)
- [ ] BRDF - IDL - Irradiance
- [ ] Basic Imgui Renderer
