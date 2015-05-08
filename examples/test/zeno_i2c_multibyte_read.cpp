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

    std::cout << "Zeno I2C interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "===================Reading Accelerometer=======================" << std::endl;
        bool success = false;
         /* 
        success = zeno_i2c_interface.writeDevice(0x53, 0x2D, 0x08);
        if (success)
        std::cout << "Write success." << std::endl;
 
        success = zeno_i2c_interface.writeDevice(0x53, 0x38, 0x82);
        if (success)
          std::cout << "Write success." << std::endl;
        */
         unsigned int status;
         std::vector<unsigned int> data;
         int read_bytes = 1;
         char start_addr = 0x20;
        /*success = zeno_i2c_interface.readDevice(0x53, 0x39, &status);
        if (success)
            std::cout << "i2c read data: "  << std::hex << status << std::endl;
        usleep(20);*/
        for(int loop=0; loop<5;loop++){ 
         success = false;
         success = zeno_i2c_interface.readDevice(0x69, start_addr, data, read_bytes);
         if (success) {
             /*
              signed int addr = start_addr;
              signed int x = data[1];
              x = (x<<8) | data[0];  
              signed int y = data[3];
              y = (y<<8) | data[2];  
              signed int z = data[5];
              z = (z<<8) | data[4];
              */
              int i=0;
              while(i<read_bytes) {
                   std::cout << "data:" << i << ":" << std::hex << data[i] << std::endl;
                   i++;
              }
              //std::cout << "acc: [" << x << "," << y << ","<< z << "]" << std::endl;  
          } else {
              std::cout << "I2C multibyte read failed."<< std::endl;
          }
          sleep(1.0);
        } 
          zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
