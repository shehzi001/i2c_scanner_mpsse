/*
 * zeno_i2c_devices_scan.cpp
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
           std::vector<unsigned int> data;
           std::cout << "===================Reading Accelerometer=======================" << std::endl;
           int read_bytes = 8;
           char start_addr = 0x30;
           bool success = zeno_i2c_interface.readDevice(0x53, start_addr, data, read_bytes);
           if (success) {
              unsigned int addr = start_addr;
              for (unsigned int i=0; i<8;i++) {
                  std::cout << std::hex << (addr+i) << ":" << std::hex << data[i] << std::endl;
              }
          } else {
              std::cout << "I2C multibyte read failed."<< std::endl;
          }
        sleep(1.0); 
        zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
