/*
 * zeno_i2c_devices_scan.cpp
 *
 * Created on: March 04, 2015
 * Author: Shehzad
 */
#include <iostream>
#include<zeno_i2c_interface/zeno_i2c_interface.h>

/* accelerometer
#define DEVICE_ID 0x53
#define ADDR_REG  0x00
*/

/* gyroscope
#define DEVICE_ID 0x69
#define ADDR_REG  0x20
*/

/* compass
#define DEVICE_ID 0x1E
#define ADDR_REG  0x02
*/

int main(int argc, char **argv) {

    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "Zeno I2C interface started" << std::endl;

    unsigned int data;
    bool success = false;
    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        sleep(1.0);
        // Reading a dummy byte.
        success = zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
        success = zeno_i2c_interface.readDevice(0x69, 0x20, &data);
        success = zeno_i2c_interface.readDevice(0x53, 0x00, &data);
        usleep(10.0);
        for(int i=0; i<2;i++) {
           data = 5;
           success = false;

           std::cout << "===================Scanning for IMU components=======================" << std::endl;
           success = zeno_i2c_interface.readDevice(0x53, 0x00, &data);
           if(success) {
             std::cout << "Acceleromerter ID:"  << std::hex << data << std::endl;
           } else {
             std::cout << "Accelerometer read failed."  << std::endl;
           }
           usleep(10.0); 
           success = false;
           success = zeno_i2c_interface.readDevice(0x69, 0x20, &data);
         
           if(success) {
             std::cout << "Gyroscope ID:"  << std::hex << data << std::endl;
           } else {
             std::cout << "Gyroscope read failed."  << std::endl;
           }
           usleep(10.0);
           success = zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
         
           if(success) {
             std::cout << "Compass ID:"  << std::hex << data << std::endl;
           } else {
             std::cout << "Compass read failed."  << std::endl;
           }
           usleep(10.0);
      }
       zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
