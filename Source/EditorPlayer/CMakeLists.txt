cmake_minimum_required (VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include("${CMAKE_SOURCE_DIR}/CMake/FetchLibraries.cmake")
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/EditorPlayer/*.cpp" "${CMAKE_SOURCE_DIR}/Source/EditorPlayer/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/EditorPlayer" FILES ${SUBDIRECTORIES})
add_executable(EditorPlayer main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(EditorPlayer PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_link_libraries(EditorPlayer PRIVATE Runtime)
target_link_libraries(EditorPlayer PRIVATE Editor)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(EditorPlayer PUBLIC EDITORPLAYER_API=__declspec\(dllexport\))
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()