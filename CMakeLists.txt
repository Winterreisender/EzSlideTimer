cmake_minimum_required(VERSION 3.20)

project(EzPptTimer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.88
)
FetchContent_MakeAvailable(imgui)
#set(imgui_SOURCE_DIR ./packages/imgui/)

find_package(OpenGL)

file(GLOB SRC
    ./src/main.cpp
    ./assets/main.rc
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
# add_compile_options(-Wall)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}

)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
)


if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
    target_link_libraries(${PROJECT_NAME}
        gdi32
        imm32
        glfw3
        opengl32
    )
elseif (APPLE)
else (UNIX)
    target_link_libraries(${PROJECT_NAME}
        glfw
        GL
    )
endif ()