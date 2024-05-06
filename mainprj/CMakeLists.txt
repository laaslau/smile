cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECTNAME cvbench)

project(${PROJECTNAME} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

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

	
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECTNAME})	


target_include_directories(${PROJECTNAME} PRIVATE
	common
	)
	
target_link_libraries(${PROJECTNAME} 
	common
	toolbox
	imgui
	gui
	)


