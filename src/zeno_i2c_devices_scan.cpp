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

#define FOUT	"eeprom.bin"	// Output file
#define WCMD	"\xA6\x00\x00"	// Write start address command
#define RCMD	"\xA7"	// Read command
#define ACK 0
int devices_address[8]= {0x03, 0x1E, 0x20, 0x48, 0x53, 0x69, 0x6D, 0x70};

class ZenoI2CInterface
{
    public:
        /**
         * Ctor.
         */
        ZenoI2CInterface(): i2c_interface_(NULL)
        {
            is_i2c_interface_open_ = false;
        }

        /**
         * Dtor.
         */
        ~ZenoI2CInterface()
        {
        }

        bool initilizeI2CInterface()
        {
            if((i2c_interface_ = MPSSE(I2C, FOUR_HUNDRED_KHZ, LSB)) != NULL && i2c_interface_->open)
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

        void sendCommand(char device_id, bool read_write) 
        {
            //char data_temp;
            if(read_write) {
                //data_temp = ((device_id << 1) | (0x01));
                Write(i2c_interface_, RCMD, sizeof(RCMD) - 1);
            } else {
                Write(i2c_interface_, WCMD, sizeof(WCMD) - 1);
                //data_temp = (device_id << 1);
            }
            //sendData(data_temp);
        }
       
        void sendData(char data)
        {
            if(is_i2c_interface_open_) {
                Write(i2c_interface_, &data, 1);
            }
        }

        char readData()
        {
            if(is_i2c_interface_open_) {   
                char *temp_data = NULL;
                temp_data = Read(i2c_interface_, 10);
                if(temp_data) {
                   std::cout << "Read Successful" << std::endl;
                   fp = fopen(FOUT, "wb");
                   if(fp)
                   {
                        fwrite(temp_data, 1, 10, fp);
                        fclose(fp);
                        printf("Dumped %d bytes to %s\n", 1, FOUT);
		   }
                   free(temp_data);
                } else {
                   std::cout << "Read Failed" << std::endl;
                }
                return *temp_data;
            }
        }

        void closeI2CInterface()
        {
            Close(i2c_interface_);
        }

    private:
        struct mpsse_context *i2c_interface_;
        bool is_i2c_interface_open_;
        char device_id;
        FILE *fp = NULL;
};

int main(int argc, char **argv) {

    ZenoI2CInterface zeno_i2c_interface;

    std::cout << "Zeno I2C interface started" << std::endl;

    if(zeno_i2c_interface.initilizeI2CInterface()) {
        std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
        sleep(1.0);

        zeno_i2c_interface.sendStartCondition();
        usleep(1000);
        zeno_i2c_interface.sendCommand(devices_address[4], false);
        
        if(zeno_i2c_interface.isACKRecieved()) {
            std::cout << "Command: Client sent ack." << std::endl;
            zeno_i2c_interface.sendData(0x00); //send internal address
            
            if(zeno_i2c_interface.isACKRecieved()) {
                std::cout << "Internal address : Client sent ack." << std::endl;
                zeno_i2c_interface.sendStartCondition();
                usleep(1000);
                zeno_i2c_interface.sendCommand(devices_address[4], true);
                
                if(zeno_i2c_interface.isACKRecieved()) {
                    std::cout << "Command after repeat start : Client sent ack." << std::endl;
                    char register_value = zeno_i2c_interface.readData();
                    std::cout << "Data read successfull" << std::hex << register_value <<std::endl;
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
        zeno_i2c_interface.closeI2CInterface();
    }

    std::cout << "Zeno I2C interface exiting" << std::endl;
    return 0;
}
