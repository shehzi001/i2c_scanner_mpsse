#!/usr/bin/env python

from mpsse import *

ACK = 1 

try:
    i2c = MPSSE(I2C, FOUR_HUNDRED_KHZ) # Initialize libmpsse for I2C operations at 400kHz
    i2c.Start() # Send the start condition
    i2c.Write("\xA7") # Write one byte to the I2C slave
    if i2c.GetAck() == ACK: # Make sure the last written byte was acknowledged
        print "Starting communication"
        i2c.Start() # Send a re-start condition
        data = i2c.Read(1) # Read one byte from the I2C slave
        print data
        i2c.SetAck(1) # Respond with a NACK for all subsequent reads
        i2c.Read(1) # Read one last "dummy" byte from the I2C slave in order to send the NACK
    print i2c.ErrorString()
    i2c.Stop() # Send I2C stop condition
    i2c.Close() 
except Exception, e:
	print "MPSSE failure:", e
