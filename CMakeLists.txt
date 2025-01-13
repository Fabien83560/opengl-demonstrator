#CMakeLists myProject
cmake_minimum_required(VERSION 3.5)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#CMAKE_BUILD_TYPE
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# Configuration du projet
project(openGLDemo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD "17")


## Lib dependency
# OpenGL
find_package(OpenGL REQUIRED)

if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    # GLX <-> Utilisation du serveur X pour le rendu
    # LEGACY <-> Utilisation Driver OpenGL
    set( OpenGL_GL_PREFERENCE "LEGACY" )
endif()


# stb_image
add_subdirectory(stbimage)
include_directories(
 	stbimage/include/
 )

# GLAD
add_subdirectory(glad)
include_directories(
	glad/include/
)

add_subdirectory (glfw-3.3.8)
include_directories(
	glfw-3.3.8/include/
)

# GLM
add_subdirectory (glm-1.0.1/glm)
include_directories(
	glm-1.0.1
)

# imgui
add_subdirectory (imgui-1.91.1)
include_directories(
	imgui-1.91.1
)

add_subdirectory(glengine)
add_subdirectory(exosTP)
