#ifndef USE_ESPHOME

#include "CWDateTimeEzTime.h"

void CWDateTimeEzTime::begin(const char *timeZone, bool use24format, const char *ntpServer = NTP_SERVER, const char *posixTZ = "")
{
  Serial.printf("[Time] NTP Server: %s, Timezone: %s\n", ntpServer, timeZone);
  ezt::setServer(String(ntpServer));

  if (strlen(posixTZ) > 1) {
    // An empty value still contains a null character so not empty is a value greater than 1.
    // Set to defined Posix TZ
    myTZ.setPosix(posixTZ);
  } else {
    // Use automatic eztime remote lookup
    myTZ.setLocation(timeZone);
  }

  this->use24hFormat = use24format;
  ezt::updateNTP();
  waitForSync(10);
}

String CWDateTimeEzTime::getFormattedTime()
{
  return myTZ.dateTime();
}

String CWDateTimeEzTime::getFormattedTime(const char *format)
{
  return myTZ.dateTime(format);
}

char *CWDateTimeEzTime::getHour(const char *format)
{
  static char buffer[3] = {'\0'};
  strncpy(buffer, myTZ.dateTime((use24hFormat ? "H" : "h")).c_str(), sizeof(buffer));
  return buffer;
}

char *CWDateTimeEzTime::getMinute(const char *format)
{
  static char buffer[3] = {'\0'};
  strncpy(buffer, myTZ.dateTime("i").c_str(), sizeof(buffer));
  return buffer;
}

int CWDateTimeEzTime::getHour()
{
  return myTZ.dateTime((use24hFormat ? "H" : "h")).toInt();
}

int CWDateTimeEzTime::getMinute()
{
  return myTZ.dateTime("i").toInt();
}

int CWDateTimeEzTime::getSecond()
{
  return myTZ.dateTime("s").toInt();
}

int CWDateTimeEzTime::getDay() 
{
  return myTZ.dateTime("d").toInt();
}
int CWDateTimeEzTime::getMonth()
{
  return myTZ.dateTime("m").toInt();
}
int CWDateTimeEzTime::getWeekday() 
{
  return myTZ.dateTime("w").toInt()-1;
}

long CWDateTimeEzTime::getMilliseconds() 
{
  return myTZ.ms(TIME_NOW);
}

bool CWDateTimeEzTime::isAM() 
{
  return myTZ.isAM();
}

bool CWDateTimeEzTime::is24hFormat() 
{
  return this->use24hFormat;
}

#endif