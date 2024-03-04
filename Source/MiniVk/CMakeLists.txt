cmake_minimum_required (VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include("${CMAKE_SOURCE_DIR}/Scripts/CMake/FetchLibraries.cmake")
# ------------------------------------------------------------

# ---------------------- CMAKE FETCH -------------------------
FetchContent_MakeAvailable(stbi)
FetchContent_MakeAvailable(glfw)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/MiniVk/*.cpp" "${CMAKE_SOURCE_DIR}/Source/MiniVk/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/MiniVk" FILES ${SUBDIRECTORIES})
add_executable(MiniVk main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(MiniVk PRIVATE "${CMAKE_SOURCE_DIR}/Source/MiniVk")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(MiniVk PUBLIC glfw)
# ------------------------------------------------------------

# ------------------ FORCE SHADER DIRECTORY ------------------
target_include_directories(MiniVk PRIVATE "${CMAKE_SOURCE_DIR}/External/")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/dxcompilerd.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/glslangd.lib")

    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/shaderc_combinedd.lib")

    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-cored.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-glsld.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-hlsld.lib")
else()
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/dxcompiler.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/glslang.lib")

    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/shaderc_combined.lib")

    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-core.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-glsl.lib")
    target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/spirv-cross-hlsl.lib")
endif()
# ------------------------------------------------------------

# ------------------- VULKAN IMPLEMENTATION ------------------
target_include_directories(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Vulkan/Include")

target_link_libraries(MiniVk PUBLIC "${CMAKE_SOURCE_DIR}/External/Libs/vulkan-1.lib")
# ------------------------------------------------------------

# ----------------- STB IMAGE IMPLEMENTATION -----------------
if (stbi_POPULATED) 
    target_compile_definitions(MiniVk PUBLIC STB_IMAGE_IMPLEMENTATION)
    target_compile_definitions(MiniVk PUBLIC STB_TRUETYPE_IMPLEMENTATION)

    target_include_directories(MiniVk PUBLIC ${stbi_SOURCE_DIR})
endif ()
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(MiniVk PUBLIC MINIVK_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(MiniVk PUBLIC MINIVK_RELEASE)
endif ()
# ------------------------------------------------------------

# ------------------ PLATFORM DEFINITIONS --------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(MiniVk PUBLIC MINIVK_PLATFORM_WINDOWS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(MiniVk PUBLIC RUNTIME_API=__declspec\(dllexport\))
    target_compile_definitions(MiniVk PUBLIC FORCEINLINE=__forceinline)
    target_compile_definitions(MiniVk PUBLIC NOINLINE=__declspec\(noinline\))
    target_compile_definitions(MiniVk PUBLIC INLINE=__inline)
    target_compile_definitions(MiniVk PUBLIC NULLPTR=decltype\(nullptr\)\(\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------