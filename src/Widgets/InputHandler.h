#ifndef SLIMSHADY_INPUTHANDLER_H
#define SLIMSHADY_INPUTHANDLER_H

#include "TextEditorWidget.h"
#include "SceneEditorWidget.h"
#include <glfw3.h>

namespace InputHandler {
    extern void ProcessHotkeyInput(GLFWwindow* window, TextEditorWidget* textEditor, SceneEditorWidget* sceneEditor);

    extern void MouseCallback(GLFWwindow *window, double xpos, double ypos);

    extern void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    extern void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};


#endif //SLIMSHADY_INPUTHANDLER_H
