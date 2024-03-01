cmake_minimum_required(VERSION 3.26)

# ------------------ PROJECT NAME ------------------
project("MINIVK-Rendering")
# --------------------------------------------------

# ------------------ CXX FLAGS ---------------------
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW) 
endif()

set(CMAKE_CXX_STANDARD 20)
# --------------------------------------------------

# --------------- COMPILER VERSIONS ----------------
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Platform is Windows, Using MSVC Compiler Version ${MSVC_VERSION}")
else()
    message(FATAL_ERROR "[CMAKE ERROR] -- UNSUPPORTED COMPILER. PLEASE CHANGE YOUR COMPILER.")
endif ()
# --------------------------------------------------

# ------------------ SUBDIRECTORIES ----------------
add_subdirectory("Source/MiniVk")
# --------------------------------------------------