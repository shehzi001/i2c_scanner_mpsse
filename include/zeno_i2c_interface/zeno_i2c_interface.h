/**
 * zeno_i2c_interface.h
 *
 * Created on: April 16, 2014
 * Author: Shehzad Ahmed
 **/

#ifndef ZENO_I2C_INTERFACE_H_
#define ZENO_I2C_INTERFACE_H_
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern "C" {
    #include <mpsse.h>  
}

#define ACK 0

class ZenoI2CInterface
{
    public:
        /**
         * Ctor.
         */
        ZenoI2CInterface(int vid=0x403, int pid=0x6011);
        /**
         * Dtor.
         */
        virtual ~ZenoI2CInterface();

        bool initilizeI2CInterface();

        bool readDevice(int device_id, char register_address, unsigned int *read_data);

        bool writeDevice(int device_id, char register_address, char data);

        void closeI2CInterface();

    private:
        bool sendStartCondition();

        bool sendStopCondition();

        bool isACKRecieved();

        bool sendAck(int ack);

        void sendCommand(int device_id, bool write_read);
       
        void sendData(char data);

        bool readData(unsigned int *read_data);

    private:
        struct mpsse_context *i2c_interface_;
        bool is_i2c_interface_open_;
        char device_id;
        int vid_;
        int pid_;
};

#endif /** ZENO_I2C_INTERFACE_H_ **/
