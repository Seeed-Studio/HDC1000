/*
 * I2C_SoftwareLibrary.h
 * Software i2c library for launchPad with msp430f5529
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
#ifndef I2C_MASTER_H
#define I2C_MASTER_H
#include "Energia.h"
#define BUFFER_LENGTH 16
class SoftwareWire {
public:
    SoftwareWire(uint8_t pinSDA, uint8_t pinSCL);
    void begin();
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(void);
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *data , size_t length);
    uint8_t requestFrom(uint8_t address, uint8_t length);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
private:
    uint8_t readI2C(uint8_t last=false);
    uint8_t readI2C(uint8_t* data, uint8_t length);
    bool restartI2C(uint8_t address, uint8_t RW);
    bool startI2C(uint8_t address, uint8_t RW);
    void stopI2C();
    bool writeI2C(uint8_t data);
    bool writeI2C(uint8_t* data, size_t length);
    uint8_t _pinSDA, _pinSCL;
    static uint8_t txAddress;
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;
    static uint8_t transmitting;
};
#endif
