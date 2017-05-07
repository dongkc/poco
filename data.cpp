// rfid reader report data
// A5 5E 11 06  54 6C AD 05  00 00 86 A0
#include <string>
#include <Poco/NumberFormatter.h>
#include "glog/logging.h"

using namespace std;
using namespace Poco;

void Crc16_Ccitt(unsigned char* ucbuf, unsigned int iLen, unsigned char* crc_code)
{
  unsigned int i,j;
  unsigned int crc = 0xFFFF;          // initial value
  unsigned int polynomial = 0x1021;   // 0001 0000 0010 0001  (0, 5, 12)

  for ( j = 0; j < iLen; ++j) {
    for ( i = 0; i < 8; i++) {
      char bit = ((ucbuf[j]   >> (7-i) & 1) == 1);
      char c15 = ((crc >> 15    & 1) == 1);
      crc <<= 1;
      if (c15 ^ bit) {
        crc ^= polynomial;
      }
    }
  }

  crc &= 0xffff;
  crc_code[0] = (unsigned char)(crc>>8);
  crc_code[1] = (unsigned char)(crc & 0x00ff);
}

int main(int argc, char *argv[])
{
  unsigned char buf[20];
  buf[0] = 0xA5;
  buf[1] = 0x5E;
  buf[2] = 0x11;
  buf[3] = 0x06;
  buf[4] = 0x54;
  buf[5] = 0x6C;
  buf[6] = 0xAD;
  buf[7] = 0x05;
  buf[8] = 0x00;
  buf[9] = 0x00;
  unsigned char crc[2];
  Crc16_Ccitt(buf, 10, crc);
  LOG(INFO) << "CRC: " << NumberFormatter::formatHex(crc[0]);
  LOG(INFO) << "CRC: " << NumberFormatter::formatHex(crc[1]);

  return 0;
}
