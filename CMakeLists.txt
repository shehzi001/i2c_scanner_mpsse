cmake_minimum_required(VERSION 2.8.3)
project(i2c_scanner_mpsse)
FIND_LIBRARY(MPSSE_LIBRARY
             NAMES libmpsse mpsse 
             PATHS /usr/lib /usr/local/lib
)

FIND_PATH(MPSSE_INCLUDES mpsse.h
          /usr/local/include
          /usr/include
)

INCLUDE_DIRECTORIES(${MPSSE_INCLUDES})

ADD_EXECUTABLE(i2c_scanner src/i2c_scan.cpp)
TARGET_LINK_LIBRARIES(i2c_scanner ${MPSSE_LIBRARY})

