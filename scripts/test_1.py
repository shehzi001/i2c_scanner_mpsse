#!/usr/bin/env python
import Adafruit_GPIO.FT232H as FT232H
     
# Temporarily disable FTDI serial drivers
FT232H.use_FT232H()
     
# Find the first FT232H device.
ft232h = FT232H.FT232H(0x403,0x6011)
     
print 'I2C read operation...'

address = 0x53
register = 0x00
frequency = 400000

# Create I2C object.
i2c = FT232H.I2CDevice(ft232h, address, frequency)

# Check if a device responds to this address.
if i2c.ping():
  print 'Found I2C device at address 0x{0:02X}'.format(address)
  response = i2c.readU8(register)
  print 'Found I2C device at address 0x{0:02X}'.format(response)

ft232h.close()
print 'Done!'
