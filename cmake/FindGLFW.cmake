set(GLFW_PATH Dependencies/GLFW)

find_path(GLFW_INCLUDE_DIR glfw3.h glfw3native.h
        PATH_SUFFIXES include
        PATHS ${GLFW_PATH})

find_library(GLFW_LIBRARY
        NAMES glfw3
        PATH_SUFFIXES lib
        PATHS ${GLFW_PATH})