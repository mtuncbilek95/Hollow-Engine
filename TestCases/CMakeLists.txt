cmake_minimum_required (VERSION 3.26 FATAL_ERROR)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/TestCases/*.cpp" "${CMAKE_SOURCE_DIR}/TestCases/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/TestCases" FILES ${SUBDIRECTORIES})
add_executable(TestCases main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ---------------------- INCLUDE DIRECTORIES ----------------
target_include_directories(TestCases PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_link_libraries(TestCases PRIVATE Engine Editor)
# ------------------------------------------------------------