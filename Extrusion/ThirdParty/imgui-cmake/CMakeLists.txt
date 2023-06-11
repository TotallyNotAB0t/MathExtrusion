CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(ImGUI)

# Adds ImGUI header
FIND_PACKAGE(Vulkan)

INCLUDE_DIRECTORIES(${ThirdParty}/imgui-cmake/Header/)
INCLUDE_DIRECTORIES(${ThirdParty}/glfw/Include/GLFW/)
LINK_DIRECTORIES(${ThirdParty}/imgui-cmake/freeTypeLib)

ADD_LIBRARY(ImGUI STATIC
        Source/imgui.cpp
		Source/imgui_demo.cpp
		Source/imgui_draw.cpp
		Source/imgui_impl_glfw.cpp
		Source/imgui_impl_vulkan.cpp
		Source/imgui_widgets.cpp
		Source/binary_to_compressed_c.cpp
		Source/imgui_freetype.cpp
		Source/imgui_stdlib.cpp
		Source/imgui_tables.cpp)

IF (VULKAN_FOUND)
	TARGET_INCLUDE_DIRECTORIES(ImGUI PRIVATE "${Vulkan_INCLUDE_DIRS}")
    TARGET_LINK_LIBRARIES(ImGUI glfw3 glfw ${Vulkan_LIBRARIES})
ELSE ()
    MESSAGE(FATAL_ERROR "Impossible de trouver Vulkan pour ImGui")
ENDIF ()

