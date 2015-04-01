/*
 * zeno_i2c_devices_scan.cpp
 *
 * Created on: March 04, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern "C" {
    #include <mpsse.h>  
}

#define ACK 0
int devices_address[8]= {0x03, 0x1E, 0x20, 0x48, 0x53, 0x69, 0x6D, 0x70};

class ZenoI2CInterface
{
    public:
        /**
         * Ctor.
         */
        ZenoI2CInterface(int vid=0x403, int pid=0x6011): i2c_interface_(NULL)
        {
            is_i2c_interface_open_ = false;
            vid_ = vid;
            pid_ = pid;
        }

        /**
         * Dtor.
         */
        ~ZenoI2CInterface()
        {
        }

        bool initilizeI2CInterface()
        {
            i2c_interface_ =  Open(vid_, pid_ , I2C, FOUR_HUNDRED_KHZ, MSB, IFACE_B, NULL);
            if(i2c_interface_->open)
            {
                is_i2c_interface_open_ = true;
                return true;
            } 
            return false;
        }

        bool sendStartCondition()
        {
            if(is_i2c_interface_open_) {
                Start(i2c_interface_);
                return true;
            }
            return false;
        }

        bool sendStopCondition()
        {
            if(is_i2c_interface_open_) {
                Stop(i2c_interface_);
                return true;
            }
            return false;
        }

        bool isACKRecieved() {
            if((GetAck(i2c_interface_) == 0) && is_i2c_interface_open_) {
                return true;
            }
            return false;
        }

        bool sendAck(int ack) {
            if(is_i2c_interface_open_) {
                SetAck(i2c_interface_, ack);
                return true;
            }
            return false;
        }

        void sendCommand(int device_id, bool write_read) 
        {
            char command;
            if(write_read) {
                command = (device_id << 1);
            } else {
                command = ((device_id << 1) | (0x01));
            }
            sendData(command);
        }
       
        void sendData(char data)
        {
            if(is_i2c_interface_open_) {
                Write(i2c_interface_, &data, 1);
            }
        }

        bool readData()
        {
            unsigned int data;
            if(is_i2c_interface_open_) {   
                char *data = NULL;
                data = Read(i2c_interface_, 1);
                if(data) {
                   std::cout << "Read Successful" << std::endl;
                   unsigned int data_temp = ((unsigned int)(*data)) & 0xFF;
                   std::cout << "i2c read data: "  << std::hex << data_temp << std::endl;
                   free(data);
                   return true; 
                } 
                   std::cout << "Read Failed" << std::endl;
            }
           return false;
        }

        void closeI2CInterface()
        {
            Close(i2c_interface_);
        }

    private:
        struct mpsse_context *i2c_interface_;
        bool is_i2c_interface_open_;
        char device_id;
        int vid_;
        int pid_;
};

int main(int argc, char **argv) {

    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "Zeno I2C interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
      for (int i=0; i<5; i++) 
       {
        zeno_i2c_interface.sendStartCondition();
        usleep(1000);
        zeno_i2c_interface.sendCommand(devices_address[4], true);
        
        if(zeno_i2c_interface.isACKRecieved()) {
            std::cout << "Command: Client sent ack." << std::endl;
            zeno_i2c_interface.sendData(0x00); //send internal address
            
            if(zeno_i2c_interface.isACKRecieved()) {
                std::cout << "Internal address : Client sent ack." << std::endl;
                zeno_i2c_interface.sendStartCondition();
                usleep(1000);
                zeno_i2c_interface.sendCommand(devices_address[4], false);
                
                if(zeno_i2c_interface.isACKRecieved()) {
                    std::cout << "Command after repeat start : Client sent ack." << std::endl;
                    bool read_success = zeno_i2c_interface.readData();
                    //std::cout << "Data read successfull" << std::hex << register_value <<std::endl;
                    zeno_i2c_interface.sendAck(1);
                    usleep(1000);
                } else {
                    std::cout << "Command after repeat start : failed" << std::endl;
                }

            } else {
                std::cout << "Internal address : failed" << std::endl;
            }

        } else {
            std::cout << "Command after start : failed" << std::endl;
        }

        usleep(1000);
        zeno_i2c_interface.sendStopCondition();
        usleep(1000);
       }
       usleep(1000);
       zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
