/*
 * imu_gyroscope.cpp
 *
 * Created on: March 28, 2015
 * Author: Shehzad
 */
#include <iostream>
#include<zeno_i2c_interface/zeno_i2c_interface.h>
#include<vector>

// gyroscope
#define DEVICE_ID 0x69
#define ADDR_REG  0x20


void imu_gyroscope() 
{
    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "IMU gyroscope interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "===================Reading Gyroscope=======================" << std::endl;
        bool success = false;
        
        // changing gyroscope mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, 0x22, 0X08);
        if (success)
           std::cout << "Gyroscope : Setup data ready signal." << std::endl;


        //Enabled block data update.
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, 0x23, 0x80);
        if (success)
          std::cout << "Gyroscope : FIFO activated." << std::endl;
        
        // changing gyroscope mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, 0x20, 0X1F);
        if (success)
           std::cout << "Gyroscope : Normal mode activated." << std::endl;
        
        // selecting FIFO enable.
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, 0x24, 0x40);
        if (success)
          std::cout << "Gyroscope : FIFO activated." << std::endl;
        // selecting streaming data mode.
        
        success = zeno_i2c_interface.writeDevice(DEVICE_ID, 0x2E, 0x50);
        if (success)
          std::cout << "Gyroscope : Streaming mode activated." << std::endl;
        
        unsigned int status;
        std::vector<unsigned int> data;
        int read_bytes = 6;
        char start_addr = 0x28;
        // Oring start address with 0x80 to move the pointer to desired location
        success = zeno_i2c_interface.readDevice(DEVICE_ID, start_addr|0x80, data, read_bytes);
       std::cout << "Gyroscope : Reading gyroscope data." << std::endl;
       for(int loop=0; loop<10;loop++){ 
         success = false;
         success = zeno_i2c_interface.readDevice(DEVICE_ID,start_addr|0x80 , data, read_bytes);
         if (success) {

             unsigned int x = data[1];
             uint16_t xx = (x<<8) | data[0];
             unsigned int y = data[3];
             uint16_t yy = (y<<8) | data[2];
             unsigned int z = data[5];
             uint16_t zz = (z<<8) | data[4];
            /* 
            std::cout << "gyro_X_L: "  << std::hex << data[0] << std::endl;
            std::cout << "gyro_X_H: "  << std::hex << data[1] << std::endl;

            std::cout << "gyro_Y_L: "  << std::hex << data[2] << std::endl;
            std::cout << "gyro_Y_H: "  << std::hex << data[3] << std::endl;
        
            std::cout << "gyro_Z_L: "  << std::hex << data[4] << std::endl;
            std::cout << "gyro_Z_H: "  << std::hex << data[5] << std::endl;
            */

            std::cout << "gyro[x,y,z]: [" << xx << "," << yy << ","<< zz << "]" << std::endl; 
          } else {
              std::cout << "I2C read failed."<< std::endl;
          }
          sleep(0.01);
       } 
          zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "IMU gyroscope interface exiting" << std::endl;
}

int main(int argc, char **argv)
{
    imu_gyroscope();
    return 0;
}
