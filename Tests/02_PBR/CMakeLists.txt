cmake_minimum_required (VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Tests/02_PBR/*.cpp" "${CMAKE_SOURCE_DIR}/Tests/02_PBR/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Tests/02_PBR" FILES ${SUBDIRECTORIES})
add_executable(02_PBR main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(02_PBR PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_include_directories(02_PBR PRIVATE "${CMAKE_SOURCE_DIR}/Tests/02_PBR")

# --------------------- LINK DIRECTORIES ----------------------
target_link_libraries(02_PBR PRIVATE Runtime)
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(02_PBR PUBLIC HOLLOW_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(02_PBR PUBLIC HOLLOW_RELEASE)
    set_target_properties(02_PBR PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif ()
# ------------------------------------------------------------

# ------------------ PLATFORM DEFINITIONS --------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(02_PBR PUBLIC HOLLOW_PLATFORM_WINDOWS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------