#include <Wire.h>

#define I2C_SLAVE_ADDRESS   	0xA0	//	we will listen on this address
#define I2C_DEBUG_CONFIRM_BYTE	0xF0	//	this byte is returned when debug mode is changed
#define I2C_ERROR				0xF2	//	code return when error encountered

/*  OPCODES FOR OUR I2C INTERFACE */

#define VALID_CODE	0x10	//	A if valid, V if invalid
#define E_W_DIR     0x20    //  East-West direction (1 char "E" or "W"
#define E_W_VEL     0x21    //  East_West velocity (2 bytes u16 in m/s * 100 )
#define N_S_DIR     0x22    //  North-South direction (1 char "N" or "S")
#define N_S_VEL     0x23    //  North-South velocity (2 bytes u16 in m/s * 100)
#define VERT_DIR    0x24    //  vertical velocity (1 char "U" or "D")
#define VERT_VEL    0x25    //  vertical velocity (2 bytes u16 in m/s *10)
#define LAT			0x40	//	return 4 bytes representing the latitude
#define LON			0x41	//	return 4 bytes representing the longitude
#define FIX_TIME	0x50	//	return the time of the most recent fix, returns 3 bytes
#define FIX_DATE	0x51	//	return the date from GPS, returns 3 bytes (year,month,day)
#define DEBUG_ON	0x60	//	turn on debugging mode, returns I2C_DEBUG_CONFIRM_BYTE
#define DEBUG_OFF	0x61	//	turn off debugging mode, returns I2C_DEBUG_CONFIRM_BYTE
#define TEST		0x02	//	flash the test LED, returns I2C_DEBUG_CONFIRM_BYTE

void setup() {
  Serial.begin(57600);
  Wire.begin();
  Serial.println("OK>");
}

void loop() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(LAT);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(I2C_SLAVE_ADDRESS,4);
  int data[3];
  for(int i = 0; i < 4; i++) {
     if( Wire.available() )
       data[i] = (byte)Wire.read(); 
  }
  Serial.print("Latitude = ");
  Serial.print((char)data[3]);
  Serial.print(" ");
  Serial.print(data[0]);
  Serial.print(" degrees ");
  Serial.print(data[1]);
  Serial.print(" minutes ");
  Serial.print(data[2]);
  Serial.println("seconds");
  delay(2000);
}
