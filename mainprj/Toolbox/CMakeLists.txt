# Toolbox
cmake_minimum_required(VERSION 3.20)
set(PROJECTNAME Toolbox)

add_library(${PROJECTNAME}
	Toolbox.cpp
	Toolbox.h
	YPLogger.h
	)

target_include_directories(${PROJECTNAME} PRIVATE
	"${PROJECT_SOURCE_DIR}/ext/log"
	)
	
