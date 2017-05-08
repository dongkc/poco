#include "RFIDWorker.h"
#include "glog/logging.h"
#include "util.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

RFIDWorker::RFIDWorker() : _activity(this, &RFIDWorker::runActivity)
{
}

void RFIDWorker::close()
{
  LOG(INFO) << "--------------------";
  google::FlushLogFiles(google::GLOG_INFO);

  _activity.stop();
  _activity.wait();
  LOG(INFO) << "--------------------";
  google::FlushLogFiles(google::GLOG_INFO);

}

bool RFIDWorker::open(std::string ip, std::string port)
{
  addr = new SocketAddress(ip, port);

  _activity.start();

  return true;
}

void RFIDWorker::runActivity()
{
  while (1) {
    try {
      socket.connect(*addr);
      LOG(INFO) << "connected";
      Timespan timeout(2000000);

      char buf[1000];

      while (!_activity.isStopped()) {
        if (!socket.poll(timeout, Socket::SELECT_READ)) {
          LOG(INFO) << "Time out";
          continue;
        }

        int num = socket.available();
        if (num) {
          socket.receiveBytes((void*)(buf + recv_bytes), num);
          LOG(INFO) << "Received bytes: " << num << " data: " << bin2hex(string(buf + recv_bytes, num));
          recv_bytes += num;
          //send_back(string(buf + 4, 6));
        } else {
          // throw exception
          LOG(INFO) << "Connection closed";
          socket.close();
          socket.connect(*addr);
        }
      }

      // exit thread;
      return;
    } catch (std::exception& e) {
      LOG(INFO) << e.what();
      Thread::sleep(2000);
    }
  }
}

int main(int argc, char *argv[])
{
  RFIDWorker worker;
  return 0;
}
