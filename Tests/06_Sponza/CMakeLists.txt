cmake_minimum_required (VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Tests/06_Sponza/*.cpp" "${CMAKE_SOURCE_DIR}/Tests/06_Sponza/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Tests/06_Sponza" FILES ${SUBDIRECTORIES})
add_executable(06_Sponza main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(06_Sponza PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# --------------------- LINK DIRECTORIES ----------------------
target_link_libraries(06_Sponza PRIVATE Runtime)
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(06_Sponza PUBLIC HOLLOW_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(06_Sponza PUBLIC HOLLOW_RELEASE)
    set_target_properties(06_Sponza PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif ()
# ------------------------------------------------------------

# ------------------ PLATFORM DEFINITIONS --------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(06_Sponza PUBLIC HOLLOW_PLATFORM_WINDOWS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------