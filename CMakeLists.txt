cmake_minimum_required(VERSION 3.0)

# Notre projet est étiqueté hello
project(interval)

# Crée des variables avec les fichiers à compiler
set(SRCS
    intervals.cpp

	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui.cpp
	imgui/imgui_impl_opengl3.cpp
	imgui/imgui_impl_sdl.cpp
    )
    
set(HEADERS

	imgui/imconfig.h
	imgui/imgui_impl_opengl3_loader.h
	imgui/imgui_internal.h
	imgui/imstb_rectpack.h
	imgui/imstb_textedit.h
	imgui/imstb_truetype.h
	imgui/imgui.h
	imgui/imgui_impl_opengl3.h
	imgui/imgui_impl_sdl.h
    )

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl -lGL -lpthread") 
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(interval ${SRCS} ${HEADERS})
target_link_libraries(interval ${SDL2_LIBRARIES})