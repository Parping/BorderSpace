cmake_minimum_required(VERSION 3.18)

# Name of project
set(PROJ_NAME Borderspace)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
    file_utils.h
    game.h
    game_object.h
    player_game_object.h
    shader.h
    geometry.h
    sprite.h
    circle.h
    timer.h
audio_manager.h
 collectible_game_object.h
enemy_game_object.h
projectile.h
HUD.h
expo_obj.h
blue_game_object.h
fortress_object.h
monster_object.h
lazer.h
bomb.h
fortress_shooter.h
effect.h
arm1.h
ract.h
bar.h
Value_Object.h
text_game_object.h
mini_map_Object.h
drawing_game_object.h
shop_obj.h
node.h
maze.h
)
 
set(SRCS
    file_utils.cpp
    game.cpp
    game_object.cpp
    main.cpp
    player_game_object.cpp
    shader.cpp
    sprite.cpp
    circle.cpp
    timer.cpp
audio_manager.cpp
collectible_game_object.cpp
enemy_game_object.cpp
projectile.cpp
HUD.cpp
expo_obj.cpp
blue_game_object.cpp
fortress_object.cpp
monster_object.cpp
lazer.cpp
bomb.cpp
fortress_shooter.cpp
effect.cpp
arm1.cpp
ract.cpp
Value_Object.cpp
text_game_object.cpp
mini_map_Object.cpp
drawing_game_object.cpp
shop_obj.cpp
node.cpp
maze.cpp
    sprite_vertex_shader.glsl
    sprite_fragment_shader.glsl
    number_fragement_shader.glsl
    bar_fragement_shader.glsl
animate_fragment_shader.glsl
child_fragment_shader.glsl
text_fragment_shader.glsl
drawing_fragment_shader.glsl
bar.cpp
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Directories to include for header files, so that the compiler can find
# path_config.h
target_include_directories(${PROJ_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Require OpenGL library
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH $ENV{COMP2501_LIBRARY_PATH} CACHE PATH "Folder with GLEW, GLFW, GLM, Folder with audio libraries and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
    find_library(OPENAL_LIBRARY openal)
    find_library(ALUT_LIBRARY alut)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
    find_library(OPENAL_LIBRARY "OpenAL32.lib" HINTS ${LIBRARY_PATH}/lib)
    find_library(ALUT_LIBRARY "alut.lib" HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})
target_link_libraries(${PROJ_NAME} ${OPENAL_LIBRARY})
target_link_libraries(${PROJ_NAME} ${ALUT_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

    # Set the default project in VS
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
endif(WIN32)

