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
        std::cout << "== Reading Compass ==" << std::endl;
       
        success = zeno_i2c_interface.writeDevice(0x1E, 0x02, 0x00);
        success = zeno_i2c_interface.writeDevice(0x1E, 0x02, 0x00);
        if (success)
          std::cout << "Write success." << std::endl;
        sleep(2.0);

        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x03, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x03, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x03, &data);
        if (success)
            std::cout << "compass_X_H: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x04, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x04, &data);
        if (success)
            std::cout << "compass_X_L: "  << std::hex << data << std::endl;

        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x07, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x07, &data);
        if (success)
            std::cout << "compass_Y_H: "  << std::hex << data << std::endl;
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x08, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x08, &data);
        if (success)
            std::cout << "compass_Y_L: "  << std::hex << data << std::endl;
        
        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x05, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x05, &data);
        if (success)
            std::cout << "compass_Z_H: "  << std::hex << data << std::endl;
        success = zeno_i2c_interface.readDevice(0x1E, 0x06, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x06, &data);
        if (success)
            std::cout << "compass_Z_L: "  << std::hex << data << std::endl;

        usleep(10.0);
        success = zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
        success = zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
        sleep(1.0); 
        zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
