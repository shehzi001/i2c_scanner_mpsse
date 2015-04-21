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

bool ZenoI2CInterface::readDevice(int device_id, char register_address, unsigned int *read_data)
{
    bool is_read_successful= false;
    int no_of_read_try_ = 2;
    for (int index=0; index < no_of_read_try_; index++) {
        sendStartCondition();
        sendCommand(device_id, true);

        if(isACKRecieved()) {
            is_read_successful = sendData(register_address);

            if(isACKRecieved()) {
                sendStartCondition();
                sendCommand(device_id, false);

                if(isACKRecieved()) {
                     is_read_successful = readData(read_data);
                     sendAck(1);
                }
            }
        }
        sendStopCondition();
        if (is_read_successful) break;
    }
    return is_read_successful;
}


bool ZenoI2CInterface::readDevice(int device_id, char register_address, std::vector<unsigned int> &read_data, int number_of_bytes)
{
    bool is_read_successful= false;
    read_data.resize(number_of_bytes);
    int no_of_read_try_ = 2;
  for (int index=0; index < no_of_read_try_; index++) {
    sendStartCondition();
    sendCommand(device_id, true);

    if(isACKRecieved()) {
        is_read_successful = sendData(register_address);

        if(isACKRecieved()) {
            sendStartCondition();
            sendCommand(device_id, false);

            if(isACKRecieved()) {
                int i=0;
                while (i < number_of_bytes) {
                    is_read_successful = readData(&read_data[i]);
                    i++;
                    if (i == number_of_bytes) {
                        sendAck(1); break;
                    } else {
                        sendAck(0);
                    }
                }

            }
        }
    }
    sendStopCondition();
   }
   return is_read_successful;
}

bool ZenoI2CInterface::writeDevice(int device_id, char register_address, char data)
{
    bool is_write_successful= false;
    sendStartCondition();
    sendCommand(device_id, true);

    if(isACKRecieved()) {
        is_write_successful = sendData(register_address);
            if(isACKRecieved()) {
                 is_write_successful = sendData(data);
            }
        }

    sendStopCondition();

    return is_write_successful;
}

bool ZenoI2CInterface::sendStartCondition()
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;

    Start(i2c_interface_);
    
    return !success;
}

bool ZenoI2CInterface::sendStopCondition()
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;

    Stop(i2c_interface_);

    return !success;
}

bool ZenoI2CInterface::isACKRecieved()
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;

    if(GetAck(i2c_interface_) == 0)
        success = true;

    return success;
}

bool ZenoI2CInterface::sendAck(int ack)
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;
    
    SetAck(i2c_interface_, ack);
    return !success;
}

bool ZenoI2CInterface::sendCommand(int device_id, bool write_read) 
{
    char command;
    if(write_read) {
        command = (device_id << 1);
    } else {
        command = ((device_id << 1) | (0x01));
    }
    return sendData(command);
}

bool ZenoI2CInterface::sendData(char data)
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;

    if(Write(i2c_interface_, &data, 1) == 0) {
        success = true;
    }

    return success;
}

bool ZenoI2CInterface::readData(unsigned int *read_data)
{
    bool success= false;

    if(!is_i2c_interface_open_) return success;

    unsigned int temp_data;
    char *data = NULL;
    data = Read(i2c_interface_, 1);
    if(data) {
       temp_data = ((unsigned int)(*data)) & 0xFF;
       *read_data = temp_data;
       success = true; 
    }
    free(data);
    return success;
}

bool ZenoI2CInterface::closeI2CInterface()
{
    bool success= false;
    if (!is_i2c_interface_open_) return success;

    Close(i2c_interface_);
    is_i2c_interface_open_ = false;
    return !success;
}
