cmake_minimum_required (VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Tests/05_EventTest/*.cpp" "${CMAKE_SOURCE_DIR}/Tests/05_EventTest/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Tests/05_EventTest" FILES ${SUBDIRECTORIES})
add_executable(05_EventTest main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(05_EventTest PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# --------------------- LINK DIRECTORIES ----------------------
target_link_libraries(05_EventTest PRIVATE Runtime)
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(05_EventTest PUBLIC HOLLOW_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(05_EventTest PUBLIC HOLLOW_RELEASE)
    set_target_properties(05_EventTest PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif ()
# ------------------------------------------------------------

# ------------------ PLATFORM DEFINITIONS --------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(05_EventTest PUBLIC HOLLOW_PLATFORM_WINDOWS=1)
else()
	message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED PLATFORM. PLEASE CHECK YOUR COMPILER AND PLATFORM.")
endif()
# ------------------------------------------------------------