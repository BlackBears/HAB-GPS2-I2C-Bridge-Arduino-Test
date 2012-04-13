#include <Wire.h>
#include <stdarg.h>

void p(char *fmt, ... ){
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

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
#define LAT			0x40 //	return 4 bytes representing the latitude
#define LON			0x41 //	return 4 bytes representing the longitude
#define FIX_TIME	0x50	// return the time of the most recent fix, returns 3 bytes
#define FIX_DATE	0x51	// return the date from GPS, returns 3 bytes (year,month,day)
#define DEBUG_ON	0x60	// turn on debugging mode, returns I2C_DEBUG_CONFIRM_BYTE
#define DEBUG_OFF	0x61	// turn off debugging mode, returns I2C_DEBUG_CONFIRM_BYTE
#define DX_MODE_ON	0x70	// turn on diagnostic mode (uses fake GPS data), returns I2C_DEBUG_CONFIRM_BYTE
#define DX_MODE_OFF	0x71	// turn off diagnostic mode, returns I2C_DEBUG_CONFIRM_BYTE
#define TEST		0x02	// flash the test LED, returns I2C_DEBUG_CONFIRM_BYTE

void setup() {
  Serial.begin(57600);
  Wire.begin();
  Serial.println("OK>");
  _dx_mode(false);
}

void loop() {
  _print_latitude();
  delay(10);
  _print_longitude();
  delay(10);
  _print_time();
  delay(2000);
}

void _dx_mode(boolean state) {
  byte data;
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write((state)?DX_MODE_ON:DX_MODE_OFF);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(I2C_SLAVE_ADDRESS,1);
  if( Wire.available() )
    data = (byte)Wire.read();
  p("DX mode set: %02X",data);
}

void _print_latitude() {
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
}

void _print_longitude() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(LON);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(I2C_SLAVE_ADDRESS,4);
  int data[3];
  for(int i = 0; i < 4; i++) {
    if( Wire.available() )
      data[i] = (byte)Wire.read(); 
  }
  Serial.print("Longitude = ");
  Serial.print((char)data[3]);
  Serial.print(" ");
  Serial.print(data[0]);
  Serial.print(" degrees ");
  Serial.print(data[1]);
  Serial.print(" minutes ");
  Serial.print(data[2]);
  Serial.println("seconds");
}

void _print_time() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(FIX_TIME);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(I2C_SLAVE_ADDRESS,6);
  int data[6];
  for(int i = 0; i < 6; i++ ) {
    if( Wire.available() )
      data[i] = (byte)Wire.read(); 
  }
  p("20%02d-%02d-%02d %02d:%02d:%02d",data[0],data[1],data[2],data[3],data[4],data[5]);
}

