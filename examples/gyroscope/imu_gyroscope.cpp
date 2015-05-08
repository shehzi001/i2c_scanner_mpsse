/*
 * imu_gyroscope.cpp
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
void imu_gyroscope() {

    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "IMU gyroscope interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        std::cout << "===================Reading Gyroscope=======================" << std::endl;
        bool success = false;
        
        // changing gyroscope mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(0x69, 0x22, 0X08);
        if (success)
           std::cout << "Gyroscope : Setup data ready signal." << std::endl;


        //Enabled block data update.
        success = zeno_i2c_interface.writeDevice(0x69, 0x23, 0x80);
        if (success)
          std::cout << "Gyroscope : FIFO activated." << std::endl;
        
        // changing gyroscope mode(sleep -> normal)
        success = zeno_i2c_interface.writeDevice(0x69, 0x20, 0X1F);
        if (success)
           std::cout << "Gyroscope : Normal mode activated." << std::endl;
        
        // selecting FIFO enable.
        success = zeno_i2c_interface.writeDevice(0x69, 0x24, 0x40);
        if (success)
          std::cout << "Gyroscope : FIFO activated." << std::endl;
        // selecting streaming data mode.
        
        success = zeno_i2c_interface.writeDevice(0x69, 0x2E, 0x50);
        if (success)
          std::cout << "Gyroscope : Streaming mode activated." << std::endl;
        
        unsigned int status;
        std::vector<unsigned int> data;
        int read_bytes = 6;
        char start_addr = 0x28;
        success = zeno_i2c_interface.readGyroDevice(0x69, start_addr, data, read_bytes);
       std::cout << "Gyroscope : Reading gyroscope data." << std::endl;
       for(int loop=0; loop<1000;loop++){ 
         success = false;
         success = zeno_i2c_interface.readDevice(0x69, start_addr|0x80 , data, read_bytes);
         if (success) {

             unsigned int x = data[1];
             x = (x<<8) | data[0];
             unsigned int y = data[3];
             y = (y<<8) | data[2];
             unsigned int z = data[5];
             z = (z<<8) | data[4];
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
          usleep(10.0);
       } 
          zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "IMU gyroscope interface exiting" << std::endl;
}

int main(int argc, char **argv) {

    imu_gyroscope();
    return 0;
}
