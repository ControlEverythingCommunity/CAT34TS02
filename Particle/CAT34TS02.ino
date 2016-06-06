// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// CAT34TS02
// This code is designed to work with the CAT34TS02_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// CAT34TS02 I2C address is 0x18(24)
#define Addr 0x18

int temp = 0;
float cTemp = 0.0, fTemp = 0.0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "CAT34TS02");
  Particle.variable("cTemp", cTemp);

  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select configuration register
  Wire.write(0x01);
  // Thermal Sensor enabled, comparator mode
  Wire.write(0x00);
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[2];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x05);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to 13-bits
  temp = ((data[0] & 0x0F) * 256) + data[1];
  if (temp > 4095)
  {
    temp -= 8192;
  }
  cTemp = temp * 0.0625;
  fTemp = (cTemp * 1.8) + 32;

  // Output data to dashboard
  Particle.publish("Temperature in Celsius : ", String(cTemp));
  Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
  delay(1000);
}
