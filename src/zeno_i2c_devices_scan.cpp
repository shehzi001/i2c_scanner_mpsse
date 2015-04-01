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
/* accelerometer
#define DEVICE_ID 0x53
#define ADDR_REG  0x00
*/

/* gyroscope
#define DEVICE_ID 0x69
#define ADDR_REG  0x20
*/

/* compass */
#define DEVICE_ID 0x1E
#define ADDR_REG  0x02


int devices_address[8]= {0x03, 0x1E, 0x20, 0x48, 0x53, 0x69, 0x6D, 0x70};

int main(int argc, char **argv) {

	struct mpsse_context *scan = NULL;
        clock_rates rates;
        
        scan = Open(0x403,0x6011, I2C, FOUR_HUNDRED_KHZ, MSB, IFACE_B, NULL);
	if(scan->open)
	{       
                std::cout << "(null) initialized at 400000Hz (I2C)" << std::endl;
		
                int addr;
		for (int device_index = 0; device_index < 6; device_index++)
		{     
			char addr_wr = DEVICE_ID << 1;
			Start(scan);
                        std::cout << "Sending slave+write command:" << Write(scan, &addr_wr, 1) << std::endl;

                        if(GetAck(scan) == ACK) {
                            addr_wr = ADDR_REG;
                            std::cout << "Sending data(internal address) : " << Write(scan, &addr_wr, 1) << std::endl;

                            if (GetAck(scan) == ACK) {
                                std::cout << "Client Response : ACK" << std::endl;
                                addr_wr = (DEVICE_ID << 1) | (0x01);
                                Start(scan);  //repeat start condition
                                std::cout << "Sending slave+read command:" << Write(scan, &addr_wr, 1) << std::endl;

                                if(GetAck(scan) == ACK) {
                                    char *data = NULL;
                                    //usleep(1000);
                                    data = Read(scan, 1);
                                    SetAck(scan, 1);
                                    unsigned int data_temp = ((unsigned int)(*data)) & 0xFF;
                                    std::cout << "i2c read data: "  << std::hex << data_temp << std::endl;
                                }
                            }
                            std::cout << std::endl;
                        }
                        //sleep(1);
                        Stop(scan);
                        //sleep(1.0);
                  }
          
	}

	Close(scan);
	return 0;
}
