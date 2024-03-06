cmake_minimum_required (VERSION 3.26)

include(FetchContent)

# GLFW Library
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.9
)

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

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)