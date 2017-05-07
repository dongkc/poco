/*
 ******************************* C SOURCE FILE *******************************
 **                                                                         **
 ** DataEncoder.cpp -
 **                                                                         **
 ** 01/18/2014 05:16:19 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2014, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#include <string>
#include <sstream>
#include <Poco/HexBinaryEncoder.h>
#include <glog/logging.h>

using namespace std;

std::string encode(const std::string &source)
{
    std::istringstream in(source);
    std::ostringstream out;
    Poco::HexBinaryEncoder hexout(out);

    // not insert new line to hexout
    hexout.rdbuf()->setLineLength(0);

    std::copy(std::istreambuf_iterator<char>(in),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(hexout));
    hexout.close();

    return out.str();
}

int main(int argc, char *argv[])
{
  char buf[2];
  buf[0] = 0x5A;
  buf[1] = 0xA5;

  LOG(INFO) << "Hex: " << encode(string(buf, 2));

  return 0;
}
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
