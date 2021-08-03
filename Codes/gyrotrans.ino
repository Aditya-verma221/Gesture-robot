#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Wire.h>

RF24 radio(9,8); // CE, CSN
const byte address[6] = "00001";
int dupa=0;
int joystick[2];

const int MPU_addr=0x68;  // I2C address of the MPU-6050
void setup(){
  Serial.begin(9600); 
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();  

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  joystick[0]=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  joystick[1]=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  joystick[2]=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  radio.write( joystick, sizeof(joystick) );
  Serial.println("send   ");
//  Serial.write(joystick,sizeof(joystick));
  

}
