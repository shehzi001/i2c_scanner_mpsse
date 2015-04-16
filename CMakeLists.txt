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

INCLUDE_DIRECTORIES(
  ${MPSSE_INCLUDES}
  include
)

ADD_EXECUTABLE(i2c_scanner
  src/i2c_scan.cpp
)
TARGET_LINK_LIBRARIES(i2c_scanner ${MPSSE_LIBRARY})

ADD_EXECUTABLE(zeno_i2c_devices_scan
  src/zeno_i2c_devices_scan.cpp
  src/zeno_i2c_interface.cpp
)
TARGET_LINK_LIBRARIES(zeno_i2c_devices_scan
  ${MPSSE_LIBRARY}
)

ADD_EXECUTABLE(imu_devices_test
  src/imu_devices_test.cpp
)
TARGET_LINK_LIBRARIES(imu_devices_test
  ${MPSSE_LIBRARY}
)
