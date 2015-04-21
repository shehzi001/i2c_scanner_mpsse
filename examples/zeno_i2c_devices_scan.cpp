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

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
           unsigned int data;
           std::cout << "===================Reading Accelerometer=======================" << std::endl;
           zeno_i2c_interface.readDevice(0x53, 0x00, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl; 
           std::cout << "===================Reading Gyroscope=======================" << std::endl;

           zeno_i2c_interface.readDevice(0x69, 0x20, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl;

           std::cout << "===================Reading Compass=======================" << std::endl;
           zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl;
       zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
