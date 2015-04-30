cmake_minimum_required(VERSION 2.8.3)
project(i2c_scanner_mpsse)

FIND_PACKAGE( Boost 1.49 COMPONENTS program_options REQUIRED )
FIND_LIBRARY(MPSSE_LIBRARY
             NAMES libmpsse mpsse 
             PATHS /usr/lib /usr/local/lib
)

FIND_PATH(MPSSE_INCLUDES mpsse.h
          /usr/local/include
          /usr/include
)


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
  ${Boost_INCLUDES}
  include
)


ADD_EXECUTABLE(zeno_imu_sensor_fusion
  examples/zeno_imu_sensor_fusion.cpp
  src/ADXL345.cpp
  src/zeno_i2c_interface.cpp
)

TARGET_LINK_LIBRARIES(zeno_imu_sensor_fusion
  ${MPSSE_LIBRARY}
  ${Boost_LIBRARY}
)
