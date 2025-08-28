/*
  RTC_DS1302.cpp - Library for reading the current date/time from the RTC DS1302 chip and correcting the date/time separately by registers
  Created by Oleg N. Andrianov, August 1, 2025.
  Released into the public domain.
*/

#include <Arduino.h>
#include "RTC_DS1302.h"

DS1302::DS1302(uint8_t pin_rst, uint8_t pin_clk, uint8_t pin_dat)
{
    _pin_rst = pin_rst;
    _pin_clk = pin_clk;
    _pin_dat = pin_dat;
}

void DS1302::init()
{
    pinMode(_pin_rst, OUTPUT);
    pinMode(_pin_clk, OUTPUT);
    pinMode(_pin_dat, OUTPUT);
    digitalWrite(_pin_rst, LOW);
    digitalWrite(_pin_clk, LOW);
    getDateTime();
    setReg(REG_SECONDS,second&0x7F);	// Пуск часов (могли стоять если батарея села)
    setReg(REG_HOUR,hour&0x7F); 	// 24-х часовой формат
}

void DS1302::getDateTime(){
    digitalWrite(_pin_rst, HIGH);
    pinMode(_pin_dat, OUTPUT); _writeByte(REG_BURST+1);
    pinMode(_pin_dat, INPUT);
    second = _bcd2dec(_readByte() & 0b01111111);
    minute = _bcd2dec(_readByte() & 0b01111111);
    hour   = _bcd2dec(_readByte() & 0b00111111);
    day    = _bcd2dec(_readByte() & 0b00111111);
    month  = _bcd2dec(_readByte() & 0b00011111);
    dow    = _bcd2dec(_readByte() & 0b00000111);
    year   = _bcd2dec(_readByte() & 0b11111111);
    digitalWrite(_pin_rst, LOW);
}


void DS1302::setReg(uint8_t Reg, uint8_t Val){		// Установка регистра
    pinMode(_pin_dat, OUTPUT);
    digitalWrite(_pin_rst, HIGH); _writeByte(REG_WP);   _writeByte(0);	   digitalWrite(_pin_rst, LOW);
    digitalWrite(_pin_rst, HIGH); _writeByte(Reg);	_writeByte(_dec2bcd(Val)); digitalWrite(_pin_rst, LOW);
}

uint8_t DS1302::_readByte()
{
    uint8_t byte = 0;

    for(uint8_t b = 0; b < 8; b++)
    {
        if (digitalRead(_pin_dat) == HIGH) byte |= 0x01 << b;
        _nextBit();
    }

    return byte;
}


void DS1302::_writeByte(uint8_t value)
{
    for(uint8_t b = 0; b < 8; b++)
    {
        digitalWrite(_pin_dat, (value & 0x01) ? HIGH : LOW);
        _nextBit();
        value >>= 1;
    }
}

void DS1302::_nextBit()
{
        digitalWrite(_pin_clk, HIGH);
        delayMicroseconds(1);

        digitalWrite(_pin_clk, LOW);
        delayMicroseconds(1);
}

uint8_t DS1302::_dec2bcd(uint8_t dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}


uint8_t DS1302::_bcd2dec(uint8_t bcd)
{
    return ((bcd / 16 * 10) + (bcd % 16));
}


