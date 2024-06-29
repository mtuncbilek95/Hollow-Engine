cmake_minimum_required(VERSION 3.26)

# ------------------ PROJECT NAME ------------------
project("Hollow-Engine")
# --------------------------------------------------

# ------------------ CXX FLAGS ---------------------
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_CXX_STANDARD 20)
endif()
# --------------------------------------------------

# --------------- COMPILER VERSIONS ----------------
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	message(STATUS "Platform is Windows.")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	message(STATUS "Platform is Linux.")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	message(STATUS "Platform is MacOS.")
else()
	message(FATAL_ERROR "Platform is not supported")
endif ()
# --------------------------------------------------

# ------------------ SUBDIRECTORIES ----------------
add_subdirectory("Source/Runtime")

add_subdirectory("Tests/01_Runtime")
add_subdirectory("Tests/02_PBR")
# --------------------------------------------------