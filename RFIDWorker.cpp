#include "RFIDWorker.h"
#include "glog/logging.h"
#include "util.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

namespace {
void Crc16_Ccitt(unsigned char* ucbuf, unsigned int iLen, unsigned char* crc_code)
{
  unsigned int i,j;
  unsigned int crc = 0xFFFF;          // initial value
  unsigned int polynomial = 0x1021;   // 0001 0000 0010 0001  (0, 5, 12)

  for ( j = 0; j < iLen; ++j)
  {
    for ( i = 0; i < 8; i++)
    {
      char bit = ((ucbuf[j]   >> (7-i) & 1) == 1);
      char c15 = ((crc >> 15    & 1) == 1);
      crc <<= 1;
      if (c15 ^ bit)
      {
        crc ^= polynomial;
      }
    }
  }

  crc &= 0xffff;
  crc_code[0] = (unsigned char)(crc>>8);
  crc_code[1] = (unsigned char)(crc & 0x00ff);
}
} // namespace

RFIDWorker::RFIDWorker() : _activity(this, &RFIDWorker::runActivity)
{
}

void RFIDWorker::close()
{
  LOG(INFO) << "--------------------";
  google::FlushLogFiles(google::GLOG_INFO);

  _activity.stop();
  //_activity.wait();
  LOG(INFO) << "--------------------";
  google::FlushLogFiles(google::GLOG_INFO);

}

bool RFIDWorker::open(std::string ip, std::string port)
{
  addr = new SocketAddress(ip, port);

  _activity.start();

  return true;
}

bool RFIDWorker::init(RFIDEventListener* pRFIDEvtListener)
{
  m_pRFIDEvtListener = pRFIDEvtListener;

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
          processFrame();
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

void RFIDWorker::processFrame()
{
  if (recv_bytes < 12) {
    return;
  }

  if (recv_buf[0] != 0xA5) {
    adjustFrame(0);
    return;
  }

  if (recv_buf[1] != 0x5E) {
    adjustFrame(1);
    return;
  }

  processBody();
}

void RFIDWorker::processBody()
{
  switch (recv_buf[2]) {
    case 0x11:
      {
        rfid_msg_t msg;
        msg.cmd = 0x11;
        msg.card_id = string((char*)recv_buf + 4, 6);
        m_pRFIDEvtListener->onRFIDStatus(msg);
        memset(recv_buf, 0, 100);
        recv_bytes = 0;
      }
      break;
    case 0x13:
      {
        rfid_msg_t msg;
        msg.cmd = 0x13;
        msg.card_id = string((char*)recv_buf + 4, 6);
        msg.card_status = recv_buf[10];
        m_pRFIDEvtListener->onRFIDStatus(msg);
        memset(recv_buf, 0, 100);
        recv_bytes = 0;
      }
      break;
    default:
      break;
  }
}

void RFIDWorker::adjustFrame(int index)
{
}

void RFIDWorker::send_back(std::string id)
{
  unsigned char buf[14];
  buf[0] = 0xA5;
  buf[1] = 0x5E;
  buf[2] = 0x12;
  buf[3] = 0x08;
  memcpy(buf + 4, id.c_str(), 6);
  buf[10] = 0x01;
  buf[11] = 0x00;

  Crc16_Ccitt(buf, 12, buf + 12);
  LOG(INFO) << "Send cmd: " << bin2hex(string((char*)buf, 14));

  socket.sendBytes(buf, 14);
}
