#pragma once
#include "CWDateTime.h"

#ifndef USE_ESPHOME

#include <ezTime.h>
#include <WiFi.h>

class CWDateTimeEzTime : public CWDateTime
{
private:
  Timezone myTZ;
  bool use24hFormat = true;

public:
  void begin(const char *timeZone, bool use24format, const char *ntpServer, const char *posixTZ);
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
  bool is24hFormat() override;
};

#endif
