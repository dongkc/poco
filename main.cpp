#include <iostream>
#include <string>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Thread.h>
#include <Poco/Timespan.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/NumberFormatter.h>
#include <glog/logging.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

int XMLArrayLength(std::string xml_path, std::string xml_key)
{
  AutoPtr<XMLConfiguration> pConf(new XMLConfiguration(xml_path));

  string value;
  int len = 0;
  for (int i = 0; i < 100; ++i) {
    string key = xml_key + "[" + NumberFormatter::format(i) + "]";
    try {
      if (i == 0) {
        value = pConf->getString(xml_key);
      } else {
        value = pConf->getString(key);
      }
    } catch(std::exception& e) {
      len = i;
      break;
    }
  }

  return len;
}

int main(int argc, char *argv[])
{
  int len = XMLArrayLength("test.xml", "prop3.prop4");
  LOG(INFO) << "len: " << len;

  AutoPtr<XMLConfiguration> pConf(new XMLConfiguration("test.xml"));
  std::string prop3 = pConf->getString("prop3.prop4[@attr]");
  LOG(INFO) << "prop3: " << prop3;
  prop3 = pConf->getString("prop3.prop4[2][@attr]");
  LOG(INFO) << "prop3: " << prop3;

  return 0;
}

int main1(int argc, char *argv[])
{
  while (1) {
    try {
      SocketAddress addr("127.0.0.1", "6000");
      StreamSocket socket;

      socket.connect(addr);
      LOG(INFO) << "connected";
      Timespan timeout(600000);

      char buf[1000];

      while (1) {
        if (!socket.poll(timeout, Socket::SELECT_READ)) {
          LOG(INFO) << "Time out";
          continue;
        }
        int num = socket.available();
        if (num) {
        LOG(INFO) << "Received bytes: " << num;
          socket.receiveBytes((void*)buf, 100);
        } else {
          // throw exception
          LOG(INFO) << "Connection closed";
          socket.close();
          LOG(INFO) << "-------------";
          socket.connect(addr);
        }
      }
    }

    catch (std::exception& e) {
      LOG(INFO) << "Exception catched";
      Thread::sleep(2000);
    }
  }

  return 0;
}
