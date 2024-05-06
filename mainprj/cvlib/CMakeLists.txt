cmake_minimum_required(VERSION 3.20)

set(ProjectName "cvlib")

# following variable will be set in env variable
set(OpenCV_DIR "c:/gitrepo/opencv_install/install/opencv")


find_package(OpenCV REQUIRED)

message(STATUS "include directory: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "library directory: ${OpenCV_LIBS}")

add_library(${ProjectName}
	Video.cpp
	Video.h
	Factory.cpp
	Factory.h
	)

target_include_directories(${ProjectName} PRIVATE
	"${OpenCV_INCLUDE_DIRS}"
	"${PROJECT_SOURCE_DIR}/ext/log"
	"${PROJECT_SOURCE_DIR}/common"
	"${PROJECT_SOURCE_DIR}/Toolbox"
	)

target_link_libraries(${ProjectName}
	${OpenCV_LIBS}
	)

set(OpenCV_BIN_DIR "${OpenCV_DIR}/x64/vc17/bin")
install(FILES 
	"${OpenCV_BIN_DIR}/$<$<CONFIG:Debug>:opencv_world490d.dll>$<$<CONFIG:Release>:opencv_world490.dll>"
	"${OpenCV_BIN_DIR}/opencv_videoio_ffmpeg490_64.dll"
	DESTINATION "${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
