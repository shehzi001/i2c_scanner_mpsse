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
        std::cout << "== Reading Gyroscope ==" << std::endl;
        
        success = zeno_i2c_interface.writeDevice(0x69, 0x20, 0x0F);
        success = zeno_i2c_interface.writeDevice(0x69, 0x20, 0x0F);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.writeDevice(0x69, 0x23, 0x80);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        
        success = zeno_i2c_interface.writeDevice(0x69, 0x24, 0x40);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.writeDevice(0x69, 0x2E, 0x50);
        if (success)
          std::cout << "Write success." << std::endl;
        usleep(10.0);
        
        //success = zeno_i2c_interface.readDevice(0x69, 0x28, &data);
        success = zeno_i2c_interface.readDevice(0x69, 0x28, &data);
        if (success)
            std::cout << "gyro_X_L: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x69, 0x29, &data);
        if (success)
            std::cout << "gyro_X_H: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x69, 0x2A, &data);
        if (success)
            std::cout << "gyro_Y_L: "  <<  std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x69, 0x2B, &data);
        if (success)
            std::cout << "gyro_Y_H: "  <<  std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x69, 0x2C, &data);
        if (success)
            std::cout << "gyro_Z_L: "  <<  std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x69, 0x2D, &data);
        if (success)
            std::cout << "gyro_Z_H: "  <<  std::hex << data << std::endl;
        usleep(10.0);
        sleep(1.0); 
        zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
