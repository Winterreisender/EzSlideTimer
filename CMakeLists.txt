cmake_minimum_required(VERSION 3.20)

project(EzPptTimer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

include(FetchContent)
FetchContent_Declare(
    ImGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.88
)
FetchContent_MakeAvailable(ImGUI)
#set(imgui_SOURCE_DIR ./packages/imgui/)


find_package(OpenGL)

file(GLOB SRC
    ./src/main.cpp
    ./assets/main.rc
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows -static -static-libgcc -static-libstdc++")
# add_compile_options(-Wall)


add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
    glfw3
    gdi32
    opengl32
    imm32
    pthread
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
)
