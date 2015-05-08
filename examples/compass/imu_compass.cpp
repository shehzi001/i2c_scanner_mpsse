/*
 * imu_accelerometer.cpp
 *
 * Created on: March 28, 2015
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
void imu_accelerometer() {

    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "IMU compass interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "===================Reading Compass=======================" << std::endl;
        bool success = false;
        // changing compass mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(0x1E, 0x02, 0x00);
        success = zeno_i2c_interface.writeDevice(0x1E, 0x02, 0x00);
        if (success)
           std::cout << "Compass : Normal mode and streaming mode activated." << std::endl;
 
        unsigned int status;
        std::vector<unsigned int> data;
        int read_bytes = 6;
        char start_addr = 0x03;
        
       std::cout << "Compass : Reading compass data." << std::endl;
       for(int loop=0; loop<1000;loop++){ 
         success = false;
         success = zeno_i2c_interface.readDevice(0x1E, start_addr, data, read_bytes);
         if (success) {
              unsigned int x = data[0];
              x = (x<<8) | data[1];
              unsigned int z = data[2];
              z = (z<<8) | data[3];  
              unsigned int y = data[4];
              y = (y<<8) | data[5];
             signed int xx;
             signed int yy;
             signed int zz;
             if(x > 2048)
                xx = x - 0xffff;
             else
                xx = x;
            
             if(y > 2048)
                yy = y - 0xffff;
             else
                yy = y;
               
             if(z > 2048)
                zz = z - 0xffff;
             else
                zz = z;
            /*             
            std::cout << "compass_X_H: "  << std::hex << data[0] << std::endl;
            std::cout << "compass_X_L: "  << std::hex << data[1] << std::endl;

            std::cout << "compass_Y_H: "  << std::hex << data[4] << std::endl;
            std::cout << "compass_Y_L: "  << std::hex << data[5] << std::endl;
        
            std::cout << "compass_Z_H: "  << std::hex << data[2] << std::endl;
            std::cout << "compass_Z_L: "  << std::hex << data[3] << std::endl;
            */      
            std::cout << "compass[x,y,z]: [" << xx << "," << yy << ","<< zz << "]" << std::endl;  
          } else {
              std::cout << "I2C read failed."<< std::endl;
          }
          usleep(10.0);
       } 
          zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "IMU compass interface exiting" << std::endl;
}

int main(int argc, char **argv) {

    imu_accelerometer();
    return 0;
}
