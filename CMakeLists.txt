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

ADD_LIBRARY(ADXL345 SHARED src/ADXL345.cpp)
ADD_LIBRARY(L3G4200D SHARED src/L3G4200D.cpp)
ADD_LIBRARY(ZENOI2CINTERFACE SHARED src/zeno_i2c_interface.cpp)
TARGET_LINK_LIBRARIES(ZENOI2CINTERFACE ${MPSSE_LIBRARY} ZENOI2CINTERFACE)


# i2c
ADD_EXECUTABLE(test_i2c_scan
  examples/test/i2c_scan.cpp
)

TARGET_LINK_LIBRARIES(test_i2c_scan
  ${MPSSE_LIBRARY}
)

ADD_EXECUTABLE(test_imu_devices
  examples/test/zeno_i2c_devices_scan.cpp
)

TARGET_LINK_LIBRARIES(test_imu_devices
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(test_multibyte_read
  examples/test/zeno_i2c_multibyte_read.cpp
)

TARGET_LINK_LIBRARIES(test_multibyte_read
  ZENOI2CINTERFACE
)

# accelerometer
ADD_EXECUTABLE(accelerometer
  examples/accelerometer/accelerometer.cpp
)

TARGET_LINK_LIBRARIES(accelerometer
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(imu_accelerometer
  examples/accelerometer/imu_accelerometer.cpp
)

TARGET_LINK_LIBRARIES(imu_accelerometer
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(zeno_imu_accelerometer
  examples/accelerometer/zeno_imu_accelerometer.cpp
)

TARGET_LINK_LIBRARIES(zeno_imu_accelerometer
  ADXL345
  ZENOI2CINTERFACE
)

# gyroscope
ADD_EXECUTABLE(gyroscope
  examples/gyroscope/gyroscope.cpp
)

TARGET_LINK_LIBRARIES(gyroscope
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(imu_gyroscope
  examples/gyroscope/imu_gyroscope.cpp
)

TARGET_LINK_LIBRARIES(imu_gyroscope
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(zeno_imu_gyro
  examples/gyroscope/zeno_imu_gyro.cpp
)

TARGET_LINK_LIBRARIES(zeno_imu_gyro
  L3G4200D
  ZENOI2CINTERFACE
)

# compass
ADD_EXECUTABLE(compass
  examples/compass/compass.cpp
)

TARGET_LINK_LIBRARIES(compass
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(imu_compass
  examples/compass/imu_compass.cpp
)

TARGET_LINK_LIBRARIES(imu_compass
  ZENOI2CINTERFACE
)

ADD_EXECUTABLE(zeno_imu_sensor_fusion
  examples/imu_data_fusion/zeno_imu_sensor_fusion.cpp
)

TARGET_LINK_LIBRARIES(zeno_imu_sensor_fusion
  ADXL345
  L3G4200D
  ZENOI2CINTERFACE
)
