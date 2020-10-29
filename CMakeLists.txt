cmake_minimum_required(VERSION 3.17)
project(SlimShady)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

add_executable(SlimShady
        src/App.cpp
        src/Frameworks/ErrorHandler.h
        src/Frameworks/ErrorHandler.cpp
        Vendor/imgui/imgui.h Vendor/imgui/imgui.cpp
        Vendor/imgui/imgui_impl_glfw.h Vendor/imgui/imgui_impl_glfw.cpp
        Vendor/imgui/imgui_impl_opengl3.h Vendor/imgui/imgui_impl_opengl3.cpp
        Vendor/imgui/imgui_draw.cpp
        Vendor/imgui/imgui_widgets.cpp src/Frameworks/Configurations.cpp src/Frameworks/Configurations.h src/Widgets/ConsoleWidget.cpp src/Widgets/ConsoleWidget.h src/Widgets/Widget.cpp src/Widgets/Widget.h Vendor/imgui/imgui_stdlib.cpp Vendor/imgui/imgui_stdlib.h src/Frameworks/Clock.cpp src/Frameworks/Clock.h Vendor/date/date.h Vendor/ImGuiColorTextEdit/TextEditor.cpp Vendor/ImGuiColorTextEdit/TextEditor.h src/Widgets/TextEditorWidget.cpp src/Widgets/TextEditorWidget.h src/Renderer.cpp src/Renderer.h src/VertexArray.cpp src/VertexArray.h src/VertexBufferLayout.cpp src/VertexBufferLayout.h src/VertexBuffer.cpp src/VertexBuffer.h src/IndexBuffer.cpp src/IndexBuffer.h src/Shader.cpp src/Shader.h src/SceneElement.cpp src/SceneElement.h src/ShaderTypes.h src/Widgets/SceneEditorWidget.cpp src/Widgets/SceneEditorWidget.h src/SceneLoader.cpp src/SceneLoader.h src/Frameworks/ShaderFileManager.cpp src/Frameworks/ShaderFileManager.h)

add_compile_definitions(GLEW_STATIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW)

find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
        tests
        Vendor
        ${GLFW_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR})

target_link_libraries(
        ${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        opengl32.lib)