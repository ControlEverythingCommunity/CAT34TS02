# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# CAT34TS02
# This code is designed to work with the CAT34TS02_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# CAT34TS02 address, 0x18(24)
# Select Configuration register, 0x01(01)
#		0x0000(00)	Thermal Sensor enabled, comparator mode
data = 0x0000
i2c.writeByte(0x18, 0x01, data)

time.sleep(0.5)

# CAT34TS02 address, 0x18(24)
# Read data back from 0x05(05), 2 bytes
# temp MSB, temp LSB
data = i2c.readBytes(0x18, 0x05, 2)

# Convert the data to 13-bits
temp = ((data[0] & 0x0F) * 256 + data[1])
if temp > 4095 :
	temp -= 8192
cTemp = temp * 0.0625
fTemp = cTemp * 1.8 + 32

# Output data to screen
print "Temperature in Celsius : %.2f C" %cTemp
print "Temperature in Fahrenheit : %.2f F" %fTemp
