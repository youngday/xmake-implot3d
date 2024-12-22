set_xmakever("2.9.0")
set_project("xmake-example")
set_languages("c++20")
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

add_requires( "nlohmann_json 3.11.3","simdutf 5.4.15","yaml-cpp 0.8.0", "toml++ 3.4.0","quill 7.5.0","fmt 11.0.2")
add_requires("matplotplusplus 1.2.1")--gnuplot 2d/3d
add_requires("imgui 1.91.5", {configs = {glfw= true,opengl3 = true,sdl2 = true}})
add_requires("glad 0.1.36")

add_packages( "nlohmann_json","simdutf","yaml-cpp", "toml++","quill","fmt")

add_includedirs("src/utils")
add_files("src/utils/*.cpp")

add_includedirs("src/")
-- 2d/3d gnu plot 
target("matplot")
    set_kind("binary")
    add_files("src/gnuplot/plot.cpp")
    add_packages("matplotplusplus")
target("matplotcsv")
    set_kind("binary")
    add_files("src/gnuplot/plot_csv.cpp")
    add_packages( "matplotplusplus")
-- implot3d
target("plot")
    set_kind("binary")
    add_files("src/plot/main_implot3d.cpp")
    add_files("src/plot/implot3d_demo.cpp")
    add_files("src/plot/implot3d/*.cpp")
    add_includedirs("src/plot/implot3d")
    add_packages("imgui","glad")


