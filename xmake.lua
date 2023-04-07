add_rules("mode.debug", "mode.release")

add_requires("imgui v1.89.4", {configs= {glfw_opengl3 = true}})
add_requires("glew 2.2.0")

-- A function to set charset of source and execution.
-- See https://github.com/xmake-io/xmake/issues/2471 for the feature request
function set_charset(charset)
    if is_plat("windows") then
        add_cxflags(string.format("/source-charset:%s"   ,charset))
        add_cxflags(string.format("/execution-charset:%s",charset))
    else
        add_cxflags(string.format("-finput-charset=%s"   ,charset))
        add_cxflags(string.format("-fexec-charset=%s"    ,charset))
    end
end

target("EzSlideTimer")
    set_kind("binary")
    set_languages("c++20")
    set_charset("UTF-8")
    add_files("src/*.cpp","src/*.rc")
    add_packages("imgui","glew")
