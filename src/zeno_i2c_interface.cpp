/**
 * zeno_i2c_interface.cpp
 *
 * Created on: April 16, 2014
 * Author: Shehzad Ahmed
 **/

#include<zeno_i2c_interface/zeno_i2c_interface.h>

/**
 * Ctor.
 */
ZenoI2CInterface::ZenoI2CInterface(int vid, int pid): i2c_interface_(NULL)
{
    is_i2c_interface_open_ = false;
    vid_ = vid;
    pid_ = pid;
}

/**
 * Dtor.
 */
ZenoI2CInterface::~ZenoI2CInterface()
{
}

bool ZenoI2CInterface::initilizeI2CInterface()
{
    i2c_interface_ =  Open(vid_, pid_ , I2C, FOUR_HUNDRED_KHZ, MSB, IFACE_B, NULL);
    if(i2c_interface_->open)
    {
        is_i2c_interface_open_ = true;
        return true;
    } 
    return false;
}

bool ZenoI2CInterface::readDevice(int device_id, char register_address, unsigned int *data)
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
                     is_read_successfull = readData(data);
                     sendAck(1);
                }
            }
        }
        sendStopCondition();
        if (is_read_successfull) break;
    }
    return is_read_successfull;
}

bool ZenoI2CInterface::writeDevice(int device_id, char register_address, char data)
{
    bool is_write_successfull= false;
    /*
     * TODO
     */

    return is_write_successfull;
}

bool ZenoI2CInterface::sendStartCondition()
{
    if(is_i2c_interface_open_) {
        Start(i2c_interface_);
        return true;
    }
    return false;
}

bool ZenoI2CInterface::sendStopCondition()
{
    if(is_i2c_interface_open_) {
        Stop(i2c_interface_);
        return true;
    }
    return false;
}

bool ZenoI2CInterface::isACKRecieved() {
    if((GetAck(i2c_interface_) == 0) && is_i2c_interface_open_) {
        return true;
    }
    return false;
}

bool ZenoI2CInterface::sendAck(int ack) {
    if(is_i2c_interface_open_) {
        SetAck(i2c_interface_, ack);
        return true;
    }
    return false;
}

void ZenoI2CInterface::sendCommand(int device_id, bool write_read) 
{
    char command;
    if(write_read) {
        command = (device_id << 1);
    } else {
        command = ((device_id << 1) | (0x01));
    }
    sendData(command);
}

void ZenoI2CInterface::sendData(char data)
{
    if(is_i2c_interface_open_) {
        Write(i2c_interface_, &data, 1);
    }
}

bool ZenoI2CInterface::readData(unsigned int *read_data)
{
    unsigned int temp_data;
    if(is_i2c_interface_open_) {   
        char *data = NULL;
        data = Read(i2c_interface_, 1);
        if(data) {
           temp_data = ((unsigned int)(*data)) & 0xFF;
           free(data);
           *read_data = temp_data;
           return true; 
        }
    } 
    return false;
}

void ZenoI2CInterface::closeI2CInterface()
{
    Close(i2c_interface_);
}
