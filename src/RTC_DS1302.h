/*
  RTC_DS1302.h - Library for reading the current date/time from the RTC DS1302 chip and correcting the date/time separately by registers
  Created by Oleg N. Andrianov, August 1, 2025.
  Released into the public domain.
*/

#ifndef _RTC_DS1302_H
#define _RTC_DS1302_H

#include <stdint.h>

#define REG_SECONDS           0x80
#define REG_MINUTES           0x82
#define REG_HOUR              0x84
#define REG_DATE              0x86
#define REG_MONTH             0x88
#define REG_DAY               0x8A
#define REG_YEAR              0x8C
#define REG_WP                0x8E
#define REG_BURST             0xBE

class DS1302
{
    public:
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t dow;

        DS1302(uint8_t pin_rst, uint8_t pin_clk, uint8_t pin_dat);	// Конфигурация пинов
        void init();							// Инициализация
        void getDateTime();						// Считывание даты/времени
        void setReg(uint8_t Reg, uint8_t Val);				// Установка регистра

    private:
        uint8_t _pin_rst;
        uint8_t _pin_clk;
        uint8_t _pin_dat;

        uint8_t _readByte();
        void _writeByte(uint8_t value);
        void _nextBit();

        uint8_t _dec2bcd(uint8_t dec);
        uint8_t _bcd2dec(uint8_t bcd);
};

#endif // _DS_1302_H

