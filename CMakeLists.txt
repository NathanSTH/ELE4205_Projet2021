cmake_minimum_required(VERSION 3.4)
add_definitions(-std=c++11)
project(client)
add_executable(client ./src/client.cpp ./src/common.cpp ./src/handleClient.cpp)

find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
	include_directories({OpenCV_INCLUDE_DIRS} /usr/include/tesseract /usr/include/leptonica)
	target_link_libraries(client ${OpenCV_LIBRARIES} tesseract lept)
else(OpenCV_FOUND)
	message(FATAL ERROR "Librairie OpenCV introuvable!")
endif(OpenCV_FOUND)

