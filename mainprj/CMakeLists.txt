cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECTNAME cvbench)

project(${PROJECTNAME} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable(${PROJECTNAME} 
	main.cpp
	)

add_compile_definitions(${PROJECTNAME} PUBLIC
	UNICODE 
	_UNICODE
	)


add_subdirectory(common)
add_subdirectory(toolbox)
add_subdirectory(imgui)
add_subdirectory(gui)
add_subdirectory(cvlib)

	
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECTNAME})	


target_include_directories(${PROJECTNAME} PRIVATE
	common
	toolbox
	imgui
	gui
	cvlib
	ext/log
	)
	
target_link_libraries(${PROJECTNAME} 
	common
	toolbox
	imgui
	gui
	cvlib
	)


