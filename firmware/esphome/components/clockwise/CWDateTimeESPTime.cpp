#include "CWDateTimeESPTime.h"

String CWDateTimeESPTime::getFormattedTime() {
    // return myTZ.dateTime();
    return "";
}

String CWDateTimeESPTime::getFormattedTime(const char *format) {
    // return myTZ.dateTime(format);
    return "";
}

char *CWDateTimeESPTime::getHour(const char *format) {
    static char buffer[3] = {'\0'};
    strncpy(buffer, rtc->now().strftime((use24hFormat ? "%H" : "%I")).c_str(),
            sizeof(buffer));
    return buffer;
}

char *CWDateTimeESPTime::getMinute(const char *format) {
    static char buffer[3] = {'\0'};
    strncpy(buffer, rtc->now().strftime("%M").c_str(), sizeof(buffer));
    return buffer;
}

int CWDateTimeESPTime::getHour() { return rtc->now().hour; }

int CWDateTimeESPTime::getMinute() { return rtc->now().minute; }

int CWDateTimeESPTime::getSecond() { return rtc->now().second; }

int CWDateTimeESPTime::getDay() { return rtc->now().day_of_month; }
int CWDateTimeESPTime::getMonth() { return rtc->now().month; }
int CWDateTimeESPTime::getWeekday() {
    return rtc->now().day_of_week - 1;
}

long CWDateTimeESPTime::getMilliseconds() {
    return 0;
}

bool CWDateTimeESPTime::isAM() { return rtc->now().hour < 12; }