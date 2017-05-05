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

int main(int argc, char *argv[])
{
  AutoPtr<XMLConfiguration> pConf(new XMLConfiguration("test.xml"));
  string value;
  int num = 0;
  for (int i = 1; i < 10; ++i) {
    string key = string("prop3.prop4[" + NumberFormatter::format(i) + "][@attr]");
    try {
    value = pConf->getString(key);
    } catch(std::exception& e) {
      LOG(INFO) << "key: " << key << " not found";
      num = i;
      break;
    }
    LOG(INFO) << "key: " << key;
  }
  LOG(INFO) << "num: " << num;

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
