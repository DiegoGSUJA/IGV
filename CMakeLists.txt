cmake_minimum_required(VERSION 3.25)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES conan_provider.cmake)
set(CONAN_HOST_PROFILE "default")

project(pr1b)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(${PROJECT_NAME}
        igvCamara.cpp
        igvCamara.h
        igvEscena3D.cpp
        igvEscena3D.h
        igvInterfaz.cpp
        igvInterfaz.h
        igvPunto3D.cpp
        igvPunto3D.h
        pr1b.cpp)

if (LINUX)
    find_path(OPENGL_REGISTRY_INCLUDE_DIRS "GL/glcorearb.h")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_REGISTRY_INCLUDE_DIRS})

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

    find_package(GLUT REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLUT::GLUT)
endif ()

if (WIN32)
    find_package(opengl_system)
    target_link_libraries(${PROJECT_NAME} opengl::opengl)

    find_package(opengl-registry)
    target_link_libraries(${PROJECT_NAME} opengl-registry::opengl-registry)

    find_package(FreeGLUT)
    target_link_libraries(${PROJECT_NAME} FreeGLUT::freeglut_static)
endif ()