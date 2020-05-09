cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

find_package(FFmpeg REQUIRED)
find_package(OpenAL REQUIRED)

include_directories(${INCLUDE_DERICTORIES} ${AVCODEC_INCLUDE_DIRS})
include_directories(${INCLUDE_DERICTORIES} ${OPENAL_INCLUDE_DIR})

set(MEDIA_SOURCES Media.cpp
                  MediaPlayer.cpp
                  Audio/AudioPlayer.cpp
                  Audio/OpenALSoundProvider.cpp)

set(MEDIA_HEADERS Media.h
                  MediaPlayer.h
                  Audio/AudioPlayer.h
                  Audio/OpenALSoundProvider.h)

if(WIN32)
else()
endif()

add_library(media STATIC ${MEDIA_SOURCES} ${MEDIA_HEADERS})
target_check_style(media)

target_link_libraries(media ${OPENAL_LIBRARY})
target_link_libraries(media ${AVCODEC_LIBRARIES} ${AVFORMAT_LIBRARIES} ${AVUTIL_LIBRARIES} ${SWSCALE_LIBRARIES} ${SWRESAMPLE_LIBRARIES})
