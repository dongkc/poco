#ifndef RFIDWORKER_H
#define RFIDWORKER_H

#include "Poco/Thread.h"
#include "Poco/Activity.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "RFIDEventListener.h"

class RFIDWorker
{
public:
  RFIDWorker();

  bool open(std::string ip, std::string port);

  void close();

protected:
  void runActivity();

private:
  void send_back(std::string id);

  void adjustFrame(int);

  void processFrame();

  void processBody();

private:
  unsigned char recv_buf[100];
  int recv_bytes;
  Poco::Net::SocketAddress* addr;
  Poco::Net::StreamSocket socket;

  Poco::Activity<RFIDWorker> _activity;
};
#endif
