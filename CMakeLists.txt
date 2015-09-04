CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

FILE(MAKE_DIRECTORY         ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

ADD_DEFINITIONS (-g)

SET(SPEAKER_SRC src/speaker.asr.c
                src/speaker.c
                src/speaker.tts.c
                src/speaker.cmd.c 
                src/speaker.cooker.c 
                src/speaker.waiter.c 
                src/speaker.main.c) 
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/3rd-libs/include)
SET (3rd-LIBS ${CMAKE_CURRENT_SOURCE_DIR}/3rd-libs/lib)

ADD_EXECUTABLE(demo ${SPEAKER_SRC})
TARGET_LINK_LIBRARIES(demo ${3rd-LIBS}/ifly/libmsc.so pthread rt dl)
