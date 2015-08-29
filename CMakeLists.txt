CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

FILE(MAKE_DIRECTORY         ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(SPEAKER_SRC speaker.asr.c speaker.cmd.c speaker.cooker.c speaker.waiter.c speaker.main.c) 
ADD_EXECUTABLE(demo ${SPEAKER_SRC})
TARGET_LINK_LIBRARIES(demo msc pthread rt dl)
