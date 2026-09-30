#ifndef CF_SIM_FAKEDATETIME_H
#define CF_SIM_FAKEDATETIME_H

// Deterministic CWDateTime for host rendering. Implements the full
// firmware CWDateTime interface so any clockface compiles against it.
#include "CWDateTime.h"

class FakeDateTime : public CWDateTime {
  int h, m;
  // getHour(format)/getMinute(format) return static buffers like ezTime.
  char hbuf[8];
  char mbuf[8];
public:
  FakeDateTime(int hour, int min) : h(hour), m(min) {
    hbuf[0] = mbuf[0] = '\0';
  }
  String getFormattedTime() override {
    char b[8];
    snprintf(b, sizeof(b), "%02d:%02d", h, m);
    return String(b);
  }
  String getFormattedTime(const char* format) override {
    (void)format;
    return getFormattedTime();
  }
  char* getHour(const char* format) override {
    (void)format;
    snprintf(hbuf, sizeof(hbuf), "%02d", h);
    return hbuf;
  }
  char* getMinute(const char* format) override {
    (void)format;
    snprintf(mbuf, sizeof(mbuf), "%02d", m);
    return mbuf;
  }
  int getHour() override { return h; }
  int getMinute() override { return m; }
  int getSecond() override { return 0; }
  long getMilliseconds() override { return 0; }
  int getDay() override { return 1; }
  int getMonth() override { return 1; }
  int getWeekday() override { return 3; }  // Wednesday
  bool isAM() override { return h < 12; }
  bool is24hFormat() override { return true; }
};

#endif
