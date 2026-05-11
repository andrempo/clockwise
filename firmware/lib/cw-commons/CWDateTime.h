#pragma once
#include <Arduino.h>

class CWDateTime
{
public:
  virtual ~CWDateTime() = default;

  virtual String getFormattedTime() = 0;
  virtual String getFormattedTime(const char* format) = 0;

  virtual char *getHour(const char *format) = 0;
  virtual char *getMinute(const char *format) = 0;
  virtual int getHour() = 0;
  virtual int getMinute() = 0;
  virtual int getSecond() = 0;
  virtual long getMilliseconds() = 0;

  virtual int getDay() = 0;
  virtual int getMonth() = 0;
  virtual int getWeekday() = 0;

  virtual bool isAM() = 0;
  virtual bool is24hFormat() = 0;
};
