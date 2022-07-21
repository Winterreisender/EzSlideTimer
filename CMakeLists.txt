cmake_minimum_required(VERSION 3.20)

project(EzPptTimer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include(FetchContent)
#FetchContent_Declare(
#    ImGUI
#    GIT_REPOSITORY https://github.com/ocornut/imgui
#    GIT_TAG v1.88
#)
#FetchContent_MakeAvailable(ImGUI)
#set(ImGUI_SOURCE_DIR ${ImGUI_SOURCE_DIR})


find_package(glfw3)
find_package(OpenGL)

set(ImGUI_SOURCE_DIR ./packages/imgui/)

message(NOTICE ${ImGUI_SOURCE_DIR})


file(GLOB SRC
    ./src/main.cpp
    ./assets/main.rc
    ${ImGUI_SOURCE_DIR}/*.cpp
    ${ImGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${ImGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
# add_compile_options(-Wall)


add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
    glfw3
    gdi32
    opengl32
    imm32
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ImGUI_SOURCE_DIR}
)
