cmake_minimum_required(VERSION 3.20)

set(ProjectName "cvlib")

# following variable will be set in env variable
set(OpenCV_DIR "c:/gitrepo/opencv_install/install/opencv")


find_package(OpenCV REQUIRED)

message(STATUS "include directory: ${OpenCV_INCLUDE_DIRS}")

add_library(${ProjectName}
	VideoDevice.cpp
	VideoDevice.h
	VideoBGRA.cpp
	VideoBGRA.h	
	SmileDetector.cpp
	SmileDetector.h		
	Data.cpp
	Data.h
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
set(OpenCV_CASCADE_DIR "${OpenCV_DIR}/etc/haarcascades")

install(FILES 
	"${OpenCV_BIN_DIR}/$<$<CONFIG:Debug>:opencv_world490d.dll>$<$<CONFIG:Release>:opencv_world490.dll>"
	"${OpenCV_BIN_DIR}/opencv_videoio_ffmpeg490_64.dll"
	"${OpenCV_CASCADE_DIR}/haarcascade_frontalface_default.xml"
	"${OpenCV_CASCADE_DIR}/haarcascade_smile.xml"
	DESTINATION "${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
