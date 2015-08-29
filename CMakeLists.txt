CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

FILE(MAKE_DIRECTORY         ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(SPEAKER_SRC src/speaker.asr.c
                src/speaker.tts.c
                src/speaker.cmd.c 
                src/speaker.cooker.c 
                src/speaker.waiter.c 
                src/speaker.main.c) 
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/3rd-libs/include)

ADD_EXECUTABLE(demo ${SPEAKER_SRC})
TARGET_LINK_LIBRARIES(demo msc pthread rt dl)
