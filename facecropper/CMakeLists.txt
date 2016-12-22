cmake_minimum_required(VERSION 3.1)

project(FaceCropper)

set(LOCAL /usr/local)

set(ZEROMQ_INCLUDE ${LOCAL}/include)
set(ZEROMQ_LIBS ${LOCAL}/lib)
set(ZEROMQ_BINS ${LOCAL}/lib)
set(ZEROMQ_LINK zmq)

# sodium
set(SODIUM_INCLUDE ${LOCAL}/include)
set(SODIUM_LIBS ${LOCAL}/lib)
set(SODIUM_BINS ${LOCAL}/lib)
set(SODIUM_LINK sodium)

# opencv
set(OPENCV_INCLUDE ${LOCAL}/include)
set(OPENCV_LIBS ${LOCAL}/lib)	
set(OPENCV_BINS ${LOCAL}/lib)	
set(OPENCV_LINK opencv_calib3d
    opencv_core
    opencv_features2d
    opencv_flann
    opencv_highgui
    opencv_imgproc
    opencv_imgcodecs
    opencv_videoio
    opencv_ml
    opencv_objdetect
    opencv_photo
    opencv_stitching
    opencv_superres
    opencv_video
    opencv_videostab)

# tbb
set(TBB_INCLUDE /usr/include)
set(TBB_LIBS /usr/lib)
set(TBB_BINS /usr/lib)
set(TBB_LINK tbb)

# protocol buffers
set(PROTOBUF_INCLUDE ${LOCAL}/include)
set(PROTOBUF_LIBS ${LOCAL}/lib)
set(PROTOBUF_BIN ${LOCAL}/bin)
set(PROTOBUF_LINK protobuf)

#ffmpeg
set(FFMPEG_INCLUDE ${LOCAL}/include)
set(FFMPEG_LIBS ${LOCAL}/lib)
set(FFMPEG_LINK avcodec avdevice avfilter avformat avutil postproc swresample swscale)

set(COMMON_BIN ${CMAKE_BINARY_DIR}/utility/common/Debug)
set(COMMON_PATH ${CMAKE_SOURCE_DIR}/utility/common)

set(CAPTURE_BIN ${CMAKE_BINARY_DIR}/utility/capture/Debug)
set(CAPTURE_PATH ${CMAKE_SOURCE_DIR}/utility/capture)

enable_testing ()

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

FILE(GLOB TARGET_H 							"include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC		 				"src/*.c*")

include_directories(include/)
include_directories(${TBB_INCLUDE})
include_directories(${OPENCV_INCLUDE})

# add library directories
link_directories(${TBB_LIBS})
link_directories(${OPENCV_LIBS})
link_directories("/usr/lib/x86_64-linux-gnu")

add_executable(${PROJECT_NAME} 	${TARGET_SRC} ${TARGET_H})

target_link_libraries(${PROJECT_NAME} ${OPENCV_LINK})
target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} boost_filesystem)
target_link_libraries(${PROJECT_NAME} boost_system)
