/*
 * readTempHumi
 * That's a example for hdc1000 
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Pillar Zuo (baozhu.zuo@seeed.cc)
 * Create Time: April 2015
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries

///
/// @brief	I2C_KIND
/// @{

#define I2C_HARDWARE 1 ///< 1=hardware
#define I2C_SOFTWARE 2 ///< 2=software

#define I2C_KIND I2C_SOFTWARE ///< Select mode

/// @}

#if (I2C_KIND == I2C_HARDWARE)

#include "Wire.h"

#else

#include "I2C_SoftwareLibrary.h"

///
/// @brief	SoftwareWire initialisation
/// @{
///
#define SCL_PIN P4_2 ///< pin for SCL
#define SDA_PIN P4_1 ///< pin for SDA
SoftwareWire Wire(SDA_PIN, SCL_PIN); ///< Instantiate SoftwareWire
/// @}

#endif

///
/// @brief	Address of the sensor
///
#define _addr 0x40

///
/// @brief	Read value
///
uint16_t _reading;

#define HDC1000_ADDR				0x40

#define HDC1000_TEMP				0x00
#define HDC1000_HUMI				0x01
#define	HDC1000_CONFIG				0x02

#define HDC1000_SERID_1				0xFB
#define HDC1000_SERID_2				0xFC
#define HDC1000_SERID_3				0xFD
#define HDC1000_MFID				0xFE
#define HDC1000_DEVID				0xFF

#define HDC1000_RST					0x80
#define	HDC1000_HEAT_ON				0x20
#define	HDC1000_HEAT_OFF			0x00
#define	HDC1000_BOTH_TEMP_HUMI		0x10
#define	HDC1000_SINGLE_MEASUR		0x00
#define	HDC1000_TEMP_HUMI_14BIT		0x00
#define	HDC1000_TEMP_11BIT			0x40
#define HDC1000_HUMI_11BIT			0x01
#define	HDC1000_HUMI_8BIT			0x02

void hdc1000_init()
{
	uint8_t config;
	config = HDC1000_RST|HDC1000_BOTH_TEMP_HUMI|HDC1000_TEMP_HUMI_14BIT|HDC1000_HEAT_ON;
	Wire.beginTransmission(_addr);
	Wire.write(HDC1000_CONFIG); 							//accessing the configuration register
	Wire.write(config); 									//sending the config bits (MSB)
	Wire.write(0x00); 										//the last 8 bits must be zeroes
	Wire.endTransmission();
}
void setReadRegister(uint8_t reg){
	Wire.beginTransmission(_addr);
	Wire.write(reg);
	Wire.endTransmission();
	
	delay(20);											 
}
uint16_t hdc1000Read16(){
	uint8_t bytes = 2;
	uint16_t dest;

	Wire.requestFrom(_addr, bytes);
	if(Wire.available()>=bytes){
		dest = Wire.read()<<8;
		dest += Wire.read();
	}
	return dest;
}

uint16_t hdc1000GetRawTemp()
{
	setReadRegister(HDC1000_TEMP);
	return hdc1000Read16();
}
uint16_t hdc1000GetRawHumi()
{
	setReadRegister(HDC1000_HUMI);
	return hdc1000Read16();
}

double hdc1000GetTemp(void){
	double temp = hdc1000GetRawTemp();

	return temp/65536.0*165.0-40.0;
}

double hdc1000GetHumi(void){
	double humi = hdc1000GetRawHumi();

	return humi/65536.0*100.0;
}
//------------------------------------------------------------------------------
///
/// @brief	setup
///
void setup(void) {
    
    Serial.begin(9600);
    Serial.println("*** START");
    
    Serial.print("I2C begin... ");
    
    Wire.begin();
    
#if (I2C_KIND == I2C_HARDWARE)
#else
#endif
    
    Serial.println("done");
    
    hdc1000_init();
    Serial.println("PUSH2 to end.");
    pinMode(PUSH2, INPUT_PULLUP);
}

///
/// @brief	loop
///
void loop(void)
{
	Serial.print("Temperature: ");
	Serial.print(hdc1000GetTemp()); 
	Serial.print("C, Humidity: ");     
	Serial.print(hdc1000GetHumi());
	Serial.println("%");
	delay(1000);    
    if (digitalRead(PUSH2)==0) {
        Serial.println("*** END");
        Serial.end();
        while (true);
    }
}




