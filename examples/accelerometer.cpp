/*
 * zeno_i2c_multibyte_read.cpp
 *
 * Created on: March 04, 2015
 * Author: Shehzad
 */
#include <iostream>
#include<zeno_i2c_interface/zeno_i2c_interface.h>
#include<vector>
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
    unsigned int data;
    bool success = false;
    std::cout << "Zeno I2C interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "== Reading Accelerometer ==" << std::endl;

        success = zeno_i2c_interface.writeDevice(0x53, 0x2D, 0x08);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.writeDevice(0x53, 0x38, 0x82);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        /*
        success = zeno_i2c_interface.readDevice(0x53, 0x39, &data);
        success = zeno_i2c_interface.readDevice(0x53, 0x39, &data);
        success = zeno_i2c_interface.readDevice(0x53, 0x39, &data);
        if (success)
            std::cout << "i2c read data: "  << std::hex << data << std::endl;
        usleep(10.0);
        */
        success = zeno_i2c_interface.readDevice(0x53, 0x32, &data);
        if (success)
            std::cout << "acc_X_L: "  <<  std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x53, 0x33, &data);
        success = zeno_i2c_interface.readDevice(0x53, 0x33, &data);
        if (success)
            std::cout << "acc_X_H: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x53, 0x34, &data);
        if (success)
            std::cout << "acc_Y_L: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x53, 0x35, &data);
        if (success)
            std::cout << "acc_Y_H: "  << std::hex << data << std::endl;
        usleep(10.0);

        success = zeno_i2c_interface.readDevice(0x53, 0x36, &data);
        if (success)
            std::cout << "acc_Z_L: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x53, 0x37, &data);
        if (success)
            std::cout << "acc_Z_H: "  << std::hex << data << std::endl;
        sleep(1.0); 
        zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
