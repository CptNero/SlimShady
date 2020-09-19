set(GLEW_PATH Dependencies/GLEW)

find_path(GLEW_INCLUDE_DIR eglew.h glew.h glxew.h wglew.h
        PATH_SUFFIXES include
        PATHS ${GLEW_PATH})

find_library(GLEW_LIBRARY
        NAMES glew32s.lib
        PATH_SUFFIXES lib
        PATHS ${GLEW_PATH})