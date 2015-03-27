/*
 * zeno_i2c_devices_scan.cpp
 *
 * Created on: March 04, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <iomanip>
#include <unistd.h>
extern "C" {
    #include <mpsse.h>  
}

#define ACK 0
int devices_address[8]= {0x03, 0x1E, 0x20, 0x48, 0x53, 0x69, 0x6D, 0x70};

class ZenoI2CInterface:
        scan(NULL)
{
    public:
        /**
         * Ctor.
         */
        ZenoI2CInterface() {
            is_i2c_interface_open_ = false;
        }

        /**
         * Dtor.
         */
        ~ZenoI2CInterface() {
            closeI2CInterface();
        }

        bool initilizeI2CInterface() {

            if((i2c_interface_ = MPSSE(I2C, FOUR_HUNDRED_KHZ, LSB)) != NULL && i2c_interface_->open)
            { 
                is_i2c_interface_open = true;
                return true;
            } else {
                return false;
            }
        }

        void closeI2CInterface() {
            Close(i2c_interface_);
        }

    private:
        struct mpsse_context *i2c_interface_;
        bool is_i2c_interface_open_;
};

int main(int argc, char **argv) {

    ZenoI2CInterface zeno_i2c_interface;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        sleep(1.0);
        zeno_i2c_interface.closeI2CInterface();
    }


    /*
    if((scan = MPSSE(I2C, FOUR_HUNDRED_KHZ, LSB)) != NULL && scan->open)
    {       
                std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        int addr;
        for (int device_index = 0; device_index < 8; device_index++)
        {       
                        addr = devices_address[device_index];
            char addr_wr = addr << 1;
            Start(scan);
                        std::cout << "writing write command:" << Write(scan, &addr_wr, 1) << std::endl;
            
           if(GetAck(scan) == ACK)
            {   std::cout << "base: (" << std::hex << (addr) << "), "<< std::endl;
                                std::cout << "wr: (" << std::hex << (addr<<1) << "), "<< std::endl;
                   
                                addr_wr = (0x01);
                                std::cout << "writing internal address:" << Write(scan, &addr_wr, 1) << std::endl;

                                if (GetAck(scan) == ACK)
                                  {                  
                                    //std::cout << "internal address recieved by the device" << std::endl;
                                    addr_wr = (addr << 1) | (0x01);
                                    Start(scan);
                                    std::cout << "writing read command:" << Write(scan, &addr_wr, 1) << std::endl;
                                    //Write(scan, &addr_wr, 1);

                                    if(GetAck(scan) == ACK)
                                      {  char *data = NULL;
                                          usleep(1000);
                                          data = Read(scan, 1);
                                          SetAck(scan, 1); 
                                          std::cout << "rd: (" << std::hex << ((addr<<1)| 0x01) << "), ";
                                      }
                                   }
                                
                                  std::cout << std::endl;
                        }
                        Stop(scan);
                        sleep(1.0); 
                }
                
    }
    Close(scan);*/
    return 0;
}
