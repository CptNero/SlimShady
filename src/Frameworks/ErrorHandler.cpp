#include <glew.h>
#include <iostream>

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() = default;

ErrorHandler::~ErrorHandler() = default;

void ErrorHandler::MessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam) {
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
    fprintf( stderr,
             "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type,
             severity,
             message);
  }
}
