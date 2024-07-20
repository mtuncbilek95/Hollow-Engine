cmake_minimum_required(VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include("${CMAKE_SOURCE_DIR}/CMake/FetchLibraries.cmake")
# ------------------------------------------------------------

# ---------------------- DATA FETCHING -----------------------
find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)
    get_filename_component(Vulkan_SDK_DIR "${Vulkan_LIBRARIES}" DIRECTORY)
    message(STATUS "Vulkan SDK found at: ${Vulkan_SDK_DIR}")
endif()

FetchContent_MakeAvailable(stbi)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(entt)
FetchContent_MakeAvailable(spirv-reflect)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(json)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Engine/*.cpp" "${CMAKE_SOURCE_DIR}/Source/Engine/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Engine" FILES ${SUBDIRECTORIES})
add_library(Engine STATIC main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

if(glfw_POPULATED)
	target_link_libraries(Engine PRIVATE glfw)
endif()

if (stbi_POPULATED) 
    target_compile_definitions(Engine PRIVATE STB_IMAGE_IMPLEMENTATION)
    target_include_directories(Engine PRIVATE ${stbi_SOURCE_DIR})
endif ()

if(assimp_POPULATED)
	target_link_libraries(Engine PRIVATE assimp)
    target_compile_definitions(Engine PRIVATE ASSIMP_LOADED=1)
endif()

if(Vulkan_FOUND)
    target_include_directories(Engine PRIVATE "${Vulkan_INCLUDE_DIRS}")
    target_link_libraries(Engine PRIVATE Vulkan::Vulkan)
    find_package(Vulkan OPTIONAL_COMPONENTS shaderc_combined)
        if (Vulkan_shaderc_combined_FOUND)
            target_link_libraries(Engine PRIVATE Vulkan::shaderc_combined)
        endif ()
endif()

if(spirv-reflect_POPULATED)
	target_include_directories(Engine PRIVATE ${spirv-reflect_SOURCE_DIR})
	target_link_libraries(Engine PRIVATE spirv-reflect-static)
endif()

if(entt_POPULATED)
	target_include_directories(Engine PRIVATE ${entt_SOURCE_DIR}/src)
endif()

if(json_POPULATED)
	target_include_directories(Engine PRIVATE ${json_SOURCE_DIR}/include)
endif()
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(Engine PUBLIC HOLLOW_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(Engine PUBLIC HOLLOW_RELEASE)
endif ()
# ------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_WINDOWS=1)
    target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_LINUX=0)
    target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_MACOS=0)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_WINDOWS=0)
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_LINUX=1)
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_MACOS=0)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_WINDOWS=0)
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_LINUX=0)
	target_compile_definitions(Engine PUBLIC HOLLOW_PLATFORM_MACOS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Engine PUBLIC ENGINE_API=__declspec\(dllexport\))
    target_compile_definitions(Engine PUBLIC FORCEINLINE=__forceinline)
    target_compile_definitions(Engine PUBLIC NOINLINE=__declspec\(noinline\))
    target_compile_definitions(Engine PUBLIC INLINE=__inline)
    target_compile_definitions(Engine PUBLIC NULLPTR=decltype\(nullptr\)\(\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------