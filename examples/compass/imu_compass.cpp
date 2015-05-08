/*
 * imu_compass.cpp
 *
 * Created on: March 28, 2015
 * Author: Shehzad
 */
#include <iostream>
#include<zeno_i2c_interface/zeno_i2c_interface.h>
#include<vector>

// compass
#define DEVICE_ID 0x1E
#define ADDR_REG  0x02

void imu_compass() 
{
    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "IMU compass interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "===================Reading Compass=======================" << std::endl;
        bool success = false;
        // changing compass mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, ADDR_REG, 0x00);
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, ADDR_REG, 0x00);
        if (success)
           std::cout << "Compass : Normal mode and streaming mode activated." << std::endl;
 
        unsigned int status;
        std::vector<unsigned int> data;
        int read_bytes = 6;
        char start_addr = 0x03;
        
       std::cout << "Compass : Reading compass data." << std::endl;
       for(int loop=0; loop<10;loop++){ 
         success = false;
         success = zeno_i2c_interface.readDevice(DEVICE_ID, start_addr, data, read_bytes);
         if (success) {
              unsigned int x = data[0];
              uint16_t xx = (x<<8) | data[1];
              unsigned int z = data[2];
              uint16_t zz = (z<<8) | data[3];  
              unsigned int y = data[4];
              uint16_t yy = (y<<8) | data[5];
            /*             
            std::cout << "compass_X_H: "  << std::hex << data[0] << std::endl;
            std::cout << "compass_X_L: "  << std::hex << data[1] << std::endl;

            std::cout << "compass_Y_H: "  << std::hex << data[4] << std::endl;
            std::cout << "compass_Y_L: "  << std::hex << data[5] << std::endl;
        
            std::cout << "compass_Z_H: "  << std::hex << data[2] << std::endl;
            std::cout << "compass_Z_L: "  << std::hex << data[3] << std::endl;
            */      
            std::cout << "compass[x,y,z]: [" <<xx << "," << yy << ","<< zz << "]" << std::endl;  
          } else {
              std::cout << "I2C read failed."<< std::endl;
          }
          sleep(0.01);
       } 
          zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "IMU compass interface exiting" << std::endl;
}

int main(int argc, char **argv) 
{
    imu_compass();
    return 0;
}
