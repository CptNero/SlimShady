cmake_minimum_required(VERSION 3.16.3)
project(SlimShady)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_executable(SlimShady
        src/App.cpp
        src/Frameworks/ErrorHandler.h
        src/Frameworks/ErrorHandler.cpp
        Vendor/stb_image/stb_image.h Vendor/stb_image/stb_image.cpp
        Vendor/stb_image/stb_image_write.h Vendor/stb_image/stb_image_write.cpp
        Vendor/imgui/imgui.h Vendor/imgui/imgui.cpp
        Vendor/imgui/imgui_impl_glfw.h Vendor/imgui/imgui_impl_glfw.cpp
        Vendor/imgui/imgui_impl_opengl3.h Vendor/imgui/imgui_impl_opengl3.cpp
        Vendor/imgui/imgui_draw.cpp
        Vendor/imgui/imgui_widgets.cpp
        src/Frameworks/Configurations.h
        src/Widgets/ConsoleWidget.cpp src/Widgets/ConsoleWidget.h
        src/Widgets/Widget.cpp src/Widgets/Widget.h
        Vendor/imgui/imgui_stdlib.cpp Vendor/imgui/imgui_stdlib.h
        src/Frameworks/Clock.cpp src/Frameworks/Clock.h
        Vendor/date/date.h
        Vendor/ImGuiColorTextEdit/TextEditor.cpp Vendor/ImGuiColorTextEdit/TextEditor.h
        src/Widgets/TextEditorWidget.cpp src/Widgets/TextEditorWidget.h
        src/Renderer.cpp src/Renderer.h
        src/VertexArray.cpp src/VertexArray.h
        src/VertexBufferLayout.cpp src/VertexBufferLayout.h
        src/VertexBuffer.cpp src/VertexBuffer.h
        src/IndexBuffer.cpp src/IndexBuffer.h
        src/Shader.cpp src/Shader.h
        src/SceneElement.cpp src/SceneElement.h
        src/ShaderType.h
        src/Widgets/SceneEditorWidget.cpp src/Widgets/SceneEditorWidget.h
        src/SceneLoader.cpp src/SceneLoader.h
        src/Frameworks/FileManager.cpp src/Frameworks/FileManager.h
        src/Camera.cpp src/Camera.h
        src/Frameworks/InputHandler.cpp src/Frameworks/InputHandler.h
        src/Widgets/WidgetBroker.h
        src/VertexAttributeType.h
        src/Frameworks/Context.cpp src/Frameworks/Context.h
        src/Widgets/FileBrowserWidget.cpp src/Widgets/FileBrowserWidget.h
        src/Texture.cpp src/Texture.h
        src/Widgets/TaskWidget.cpp src/Widgets/TaskWidget.h
        src/Frameworks/ImageComparator.cpp src/Frameworks/ImageComparator.h
        )

find_package(glfw3)
find_package(GLEW)
find_package(OpenGL)

target_include_directories(${PROJECT_NAME} PUBLIC
        Vendor
        ${GLFW3_INCLUDEDIR/GLFW/}
        ${GLEW_INCLUDEDIR/GLEW/})

target_link_libraries(
        ${PROJECT_NAME}
        glfw
        GLEW
        OpenGL::GL
        ${CMAKE_DL_LIBS}
)