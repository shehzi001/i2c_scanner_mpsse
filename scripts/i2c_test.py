#!/usr/bin/env python

from mpsse import *
from decimal import *

SIZE = 0x0001		# Size of EEPROM chip (32 KB)
WCMD = "\xA6"	# Write start address command
RCMD = "\xA7"		# Read command
FOUT = "i2c_imu.bin"	# Output file
ACK = 0

try:
	i2c_imu = MPSSE(I2C, FOUR_HUNDRED_KHZ, LSB)

	print "%s initialized at %dHz (I2C)" % (i2c_imu.GetDescription(), i2c_imu.GetClock())

	i2c_imu.Start()
	i2c_imu.Write(WCMD)
        
	if i2c_imu.GetAck() == ACK:
           i2c_imu.Write("\x2C")
           if i2c_imu.GetAck() == ACK:
		i2c_imu.Start()
		i2c_imu.Write(RCMD)
	
		if i2c_imu.GetAck() == ACK:
			data = i2c_imu.Read(SIZE)
			i2c_imu.SetAck(1)
                        print data

			#i2c_imu.Read(1)
		else:
			raise Exception("Received read command NACK!")
	else:
		raise Exception("Received write command NACK!")

	i2c_imu.Stop()
	
	open(FOUT, "wb").write(data)	
	print "Dumped %d bytes to %s" % (len(data), FOUT)
	
	i2c_imu.Close()
except Exception, e:
	print "MPSSE failure:", e
