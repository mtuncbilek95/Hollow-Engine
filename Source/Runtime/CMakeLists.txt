cmake_minimum_required(VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Runtime/*.cpp" "${CMAKE_SOURCE_DIR}/Source/Runtime/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Runtime" FILES ${SUBDIRECTORIES})
add_executable(Runtime main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Runtime PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(Runtime PUBLIC HOLLOW_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(Runtime PUBLIC HOLLOW_RELEASE)
endif ()
# ------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_WINDOWS=1)
    target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_LINUX=0)
    target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_MACOS=0)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_WINDOWS=0)
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_LINUX=1)
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_MACOS=0)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_WINDOWS=0)
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_LINUX=0)
	target_compile_definitions(Runtime PUBLIC HOLLOW_PLATFORM_MACOS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Runtime PUBLIC RUNTIME_API=__declspec\(dllexport\))
    target_compile_definitions(Runtime PUBLIC FORCEINLINE=__forceinline)
    target_compile_definitions(Runtime PUBLIC NOINLINE=__declspec\(noinline\))
    target_compile_definitions(Runtime PUBLIC INLINE=__inline)
    target_compile_definitions(Runtime PUBLIC NULLPTR=decltype\(nullptr\)\(\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------