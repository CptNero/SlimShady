#ifndef OPENGLTEMPLATE_ERRORHANDLER_H
#define OPENGLTEMPLATE_ERRORHANDLER_H

#include <GL/glew.h>

class ErrorHandler {
  public:
    ErrorHandler();
    ~ErrorHandler();

    static void GLAPIENTRY MessageCallback(
      GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar* message,
      const void* userParam);
};

#endif //OPENGLTEMPLATE_ERRORHANDLER_H
