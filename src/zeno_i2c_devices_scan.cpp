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

        bool readDevice(int device_id, char register_address, unsigned int *read_data)
        {
            bool is_read_successfull= false;
            int no_of_read_try_ = 2;
            for (int index=0; index < no_of_read_try_; index++) {
                sendStartCondition();
                sendCommand(device_id, true);
        
                if(isACKRecieved()) {
                    sendData(register_address);          
                    if(isACKRecieved()) {
                        sendStartCondition();
                        sendCommand(device_id, false);
 
                        if(isACKRecieved()) {
                             is_read_successfull = readData(read_data);
                             sendAck(1);
                    } 
                } 
            } 
            sendStopCondition();
            if (is_read_successfull) break;
          }
            return is_read_successfull;
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

        bool readData(unsigned int *read_data)
        {
            unsigned int temp_data;
            if(is_i2c_interface_open_) {   
                char *data = NULL;
                data = Read(i2c_interface_, 1);
                if(data) {
                   //std::cout << "Read Successful" << std::endl;
                   temp_data = ((unsigned int)(*data)) & 0xFF;
                   //std::cout << "i2c read data: "  << std::hex << temp_data << std::endl;
                   free(data);
                   *read_data = temp_data;
                   return true; 
                }
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
           unsigned int data;
           std::cout << "===================Reading Accelerometer=======================" << std::endl;
           zeno_i2c_interface.readDevice(0x53, 0x00, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl; 
           std::cout << "===================Reading Gyroscope=======================" << std::endl;

           zeno_i2c_interface.readDevice(0x69, 0x20, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl;

           std::cout << "===================Reading Compass=======================" << std::endl;
           zeno_i2c_interface.readDevice(0x1E, 0x02, &data);
           std::cout << "i2c read data: "  << std::hex << data << std::endl;
       zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
