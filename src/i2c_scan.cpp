/*
 * i2c_scan.cpp
 *
 * Created on: March 04, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <iomanip>

extern "C" {
    #include <mpsse.h>	
}

#define ACK 0

int main(int argc, char **argv) {

	struct mpsse_context *scan = NULL;

	if((scan = MPSSE(I2C, FOUR_HUNDRED_KHZ, LSB)) != NULL && scan->open)
	{
		int addr;
		for (addr = 0; addr < 0x7F; ++addr)
		{
			char addr_wr = addr << 1;
			Start(scan);
			Write(scan, &addr_wr, 1);

			if(GetAck(scan) == ACK)
			{
				
				std::cout << "Device found at: " << std::hex << (addr<<1) << std::endl;
			}
			else
			{
				//std::cout << "Not device found at: " << std::hex << addr<<1 << std::endl;
			}
			Stop(scan);
		}
	}
	Close(scan);

	return 0;
}
