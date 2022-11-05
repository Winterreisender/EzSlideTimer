
add_rules("mode.debug", "mode.release")
add_requires("imgui v1.88", {configs = {glfw_opengl3 = true}} )

target("EzPptTimer")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("imgui")
    add_links("opengl32","imm32","gdi32")
    add_toolchains("mingw")