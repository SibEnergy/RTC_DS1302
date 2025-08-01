#include <RTC_DS1302.h>

#define RTC_CLK 25
#define RTC_DAT 26
#define RTC_RST 27

DS1302 rtc(RTC_RST, RTC_CLK, RTC_DAT);

uint8_t past_sec;

void setup() {
  Serial.begin(115200);
  rtc.init();
  rtc.setReg(REG_HOUR,rtc.hour&0x7F); // Set 24-hour format (need once). The list of register names is in the Ds1302.h file.
}

void loop() {
    rtc.getDateTime();
    if(rtc.second!=past_sec){
        past_sec=rtc.second;
        Serial.printf("%d-%02d-%02d %02d:%02d:%02d",2000+rtc.year,rtc.month,rtc.day,rtc.hour,rtc.minute,rtc.second);
    }
    delay(10);
}
