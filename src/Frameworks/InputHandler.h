#ifndef SLIMSHADY_INPUTHANDLER_H
#define SLIMSHADY_INPUTHANDLER_H

#include "../Widgets/TextEditorWidget.h"
#include "../Widgets/SceneEditorWidget.h"
#include <glfw3.h>

namespace InputHandler {
    extern TextEditorWidget* textEditor;
    extern SceneEditorWidget* sceneEditor;

    extern void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    extern void MouseCallback(GLFWwindow *window, double xpos, double ypos);

    extern void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    extern void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};


#endif //SLIMSHADY_INPUTHANDLER_H
