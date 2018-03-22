#ifndef COM_LAYER
#define COM_LAYER

#include "application.h"


//WKC - wireless krembot communication protocol
class COMLayer
{
private:
  bool connected_;
  TCPClient client_;
  void stringToIp(const char *ip_str, byte ip_arr[]);

public:
  COMLayer();
  bool connect(const char* ip, uint16_t port);
  bool isConnected();
  bool bytesWaiting();
  void write(byte val);
  void write(byte buff[], size_t len);
  byte read();
  uint32_t read(byte buff[], size_t len);
  void disconnect();

};

#endif
