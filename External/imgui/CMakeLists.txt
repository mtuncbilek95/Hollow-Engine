cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# IMGUI Library
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG docking
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_FILES "${imgui_SOURCE_DIR}/*.cpp" "${imgui_SOURCE_DIR}/*.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
source_group(TREE "${imgui_SOURCE_DIR}" FILES ${IMGUI_FILES})
add_library(Imgui STATIC ${IMGUI_FILES})

target_include_directories(Imgui PUBLIC ${imgui_SOURCE_DIR})

target_include_directories(Imgui PUBLIC ${glfw_INCLUDE_DIR})
target_link_libraries(Imgui PUBLIC glfw)

include(SetupVulkan)
SetupVulkan(Imgui)