cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# GLFW Library
FetchContent_Declare(
        glm
		EXCLUDE_FROM_ALL
		GIT_REPOSITORY	https://github.com/g-truc/glm.git
		GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
        GIT_SHALLOW TRUE
)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)