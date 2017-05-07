#ifndef RFIDWORKER_H
#define RFIDWORKER_H

#include "Poco/Thread.h"
#include "Poco/Activity.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "RFIDEventListener.h"
#include "message.h"

class RFIDWorker
{
public:
  RFIDWorker();

  bool init(RFIDEventListener* pRFIDEvtListener);

  bool open(std::string ip, std::string port);

  void close();

  void process(card_msg_result_t* msg);

  void process(card_msg_collect_t* msg);

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
  RFIDEventListener* m_pRFIDEvtListener;

  Poco::Activity<RFIDWorker> _activity;
};
#endif
