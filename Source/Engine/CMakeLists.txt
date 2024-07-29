cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# ----------------------- INCLUDES ---------------------------
include(SetupVulkan)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Engine/*.cpp" "${CMAKE_SOURCE_DIR}/Source/Engine/*.h")
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Engine" FILES ${SUBDIRECTORIES})
add_library(Engine STATIC main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

SetupVulkan(Engine)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PUBLIC nlohmann_json::nlohmann_json)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(WIN32)
endif()
# ------------------------------------------------------------

