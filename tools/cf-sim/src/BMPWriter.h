#ifndef HOST_BMPWRITER_H
#define HOST_BMPWRITER_H
#include <cstdint>
bool writeBMP24(const char* path, const uint16_t* rgb565, int w, int h);
#endif
