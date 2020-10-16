cmake_minimum_required(VERSION 3.17)
project(SlimShady)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

add_executable(SlimShady
        src/App.cpp
        src/tests/Test.h
        src/tests/Test.cpp
        src/tests/TestClearColor.h
        src/tests/TestClearColor.cpp
        src/ErrorHandler.h
        src/ErrorHandler.cpp
        vendor/imgui/imgui.h vendor/imgui/imgui.cpp
        vendor/imgui/imgui_impl_glfw.h vendor/imgui/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl_opengl3.h vendor/imgui/imgui_impl_opengl3.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp src/Configurations.cpp src/Configurations.h src/Ui/ConsoleWidget.cpp src/Ui/ConsoleWidget.h src/Ui/Widget.cpp src/Ui/Widget.h vendor/imgui/imgui_stdlib.cpp vendor/imgui/imgui_stdlib.h src/Clock.cpp src/Clock.h vendor/date/date.h vendor/ImGuiColorTextEdit/TextEditor.cpp vendor/ImGuiColorTextEdit/TextEditor.h src/Ui/TextEditorWidget.cpp src/Ui/TextEditorWidget.h)

add_compile_definitions(GLEW_STATIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW)

find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
        tests
        vendor
        ${GLFW_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR})

target_link_libraries(
        ${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        opengl32.lib)