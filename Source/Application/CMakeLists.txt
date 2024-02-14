cmake_minimum_required (VERSION 3.26)

# ---------------------- CMAKE INCLUDES ----------------------
include("${CMAKE_SOURCE_DIR}/Scripts/CMake/FetchLibraries.cmake")
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Application/*.cpp" "${CMAKE_SOURCE_DIR}/Source/Application/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Application" FILES ${SUBDIRECTORIES})
add_executable(Application main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Application PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(Application PRIVATE Runtime)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(Application PUBLIC GAME_API=__declspec\(dllexport\))
else()
    target_compile_definitions(Application PUBLIC GAME_API=)
endif()
# ------------------------------------------------------------