// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// CAT34TS02
// This code is designed to work with the CAT34TS02_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products


import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class CAT34TS02
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, CAT34TS02 I2C address is 0x18(24)
		I2CDevice device = Bus.getDevice(0x18);
		
		// Select configuration register, 0x01(01)
		// Thermal Sensor enabled, comparator mode
		byte[] config = new byte[2];
		config[0] = 0x00;
		config[1] = 0x00;
		device.write(0x01, config, 0, 2);
		Thread.sleep(500);
		
		//Read 2 bytes of data
		// cTemp msb, cTemp lsb
		byte[] data = new byte[2];
		device.read(0x05, data, 0, 2);
		
		// Convert the data to 13-bits
		int temp = (((data[0] & 0x0F) * 256) + (data[1] & 0xFF));
		if(temp > 4095)
		{
			temp -= 8192;
		}
		double cTemp = temp * 0.0625;
		double fTemp = (cTemp * 1.8) + 32;
		
		// Output data to screen
		System.out.printf("Temperature in Celsius : %.2f C %n", cTemp);
		System.out.printf("Temperature in Fahrenheit : %.2f F %n", fTemp);
	}
}