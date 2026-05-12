#pragma once
#include "esphome/components/time/real_time_clock.h"
#include "CWDateTime.h"

class CWDateTimeESPTime : public CWDateTime
{
private:
  esphome::time::RealTimeClock *rtc;
  bool use24hFormat = true;

public:
  CWDateTimeESPTime(esphome::time::RealTimeClock *rtc)  {
    this->rtc = rtc;
  }

  String getFormattedTime() override;
  String getFormattedTime(const char* format) override;

  char *getHour(const char *format) override;
  char *getMinute(const char *format) override;
  int getHour() override;
  int getMinute() override;
  int getSecond() override;
  long getMilliseconds() override;

  int getDay() override;
  int getMonth() override;
  int getWeekday() override;

  bool isAM() override;
  bool is24hFormat() override { return use24hFormat; }

};
