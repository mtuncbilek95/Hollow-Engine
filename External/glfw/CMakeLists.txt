cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# GLFW Library
FetchContent_Declare(
        glfw
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.9
        GIT_SHALLOW TRUE
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)