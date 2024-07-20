cmake_minimum_required(VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include("${CMAKE_SOURCE_DIR}/CMake/FetchLibraries.cmake")
# ------------------------------------------------------------

FetchContent_MakeAvailable(imgui)

# ---------------------- IMGUI ADDITION ----------------------
file(GLOB IMGUI_FILES "${imgui_SOURCE_DIR}/*.cpp" "${imgui_SOURCE_DIR}/*.h")
source_group(TREE "${imgui_SOURCE_DIR}" FILES ${IMGUI_FILES})
add_library(Imgui STATIC ${IMGUI_FILES})


# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Editor/*.cpp" "${CMAKE_SOURCE_DIR}/Source/Editor/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Editor" FILES ${SUBDIRECTORIES})
add_library(Editor main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Editor PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_include_directories(Editor PRIVATE "${imgui_SOURCE_DIR}")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(Editor PRIVATE Engine)
target_link_libraries(Editor PRIVATE Imgui)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Engine PUBLIC EDITOR_API=__declspec\(dllexport\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------