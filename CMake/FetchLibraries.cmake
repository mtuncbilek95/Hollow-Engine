cmake_minimum_required (VERSION 3.26)

include(FetchContent)

# STBI Library
FetchContent_Declare(
        stbi
        GIT_REPOSITORY https://github.com/nothings/stb.git
)

# JSON Library
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
)

# ASSIMP Library
FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
)

# IMGUI Library
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG docking
)

# SPIRV-REFLECT Library
FetchContent_Declare(
    spirv-reflect
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Reflect.git
    GIT_TAG main
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Assimp build tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Assimp build samples" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Assimp build all importers by default" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Assimp build all exporters by default" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Assimp build OBJ importer" FORCE)

set(SPIRV_REFLECT_STATIC_LIB TRUE)

