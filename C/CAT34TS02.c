// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// CAT34TS02
// This code is designed to work with the CAT34TS02_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, AMS5812 I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Select configuration register(0x01)
	// Thermal Sensor enabled, comparator mode(0x00, 0x00)
	char config[3] = {0};
	config[0] = 0x01;
	config[1] = 0x00;
	config[2] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x05)
	// cTemp msb, cTemp lsb
	char reg[1] = {0x05};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else 
	{
		// Convert the data
		int temp = ((data[0] & 0x0F) * 256 + data[1]);
		if(temp > 4095)
		{
			temp -= 8192;
		}
		float cTemp = temp * 0.0625;
		float fTemp = (cTemp * 1.8) + 32;

		// Output data to screen
		printf("Temperature in Celsius : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	}
}
