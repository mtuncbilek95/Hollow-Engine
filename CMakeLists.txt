cmake_minimum_required(VERSION 3.26)

# ----------------------- PROJECT NAME -----------------------
project("Hollow-Engine")
# ------------------------------------------------------------

# ----------------------- CXX FLAGS --------------------------
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_CXX_STANDARD 20)
endif()
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
# ------------------------------------------------------------

# --------------------- SETUP CONFIG -------------------------
include(SetupConfig)
SetupConfig()
# ------------------------------------------------------------

# ----------------------- SUBDIRECTORIES ---------------------
add_subdirectory("External")
add_subdirectory("Source/Engine")
add_subdirectory("Source/Resource")
add_subdirectory("Source/Editor")

add_subdirectory("TestCases")
# ------------------------------------------------------------