
#include "com_layer.h"

/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

/*
  *    Constructor
  *
  */
COMLayer::COMLayer()
{
  connected_ = false;
}

/*
  *   brief Connect to the server
  *  
  *   param ip - the ip of the server, port - the port of the server
  *   
  *   return true if the client connected successfully, false otherwise
  *
  */

bool COMLayer::connect(const char* ip, uint16_t port)
{
  //TODO: check valid port (return false if not)
  byte ip_arr[4] = {0};
  stringToIp(ip, ip_arr);

  return client_.connect(ip_arr, port);
}
/* 
  *   brief Checks if the client is connected to the server
  *  
  *   param 
  *   
  *   return true if the client is connected, false otherwise
  *
  */


bool COMLayer::isConnected() { return client_.connected(); }

/*
  *   brief Checks if the client is disconnected to the server
  *  
  *   param 
  *   
  *   return true if the client is disconnected, false otherwise
  *
  */

void COMLayer::disconnect() { client_.stop(); }

/* 
  *   brief Checks if the there are bytes waiting in the buffer
  *  
  *   param 
  *   
  *   return true if there are bytes waiting, false otherwise
  *
  */

bool COMLayer::bytesWaiting() { return client_.available(); }

/* 
  *   brief Write 1 byte to the server
  *  
  *   param val - the byte to be written
  *   
  *   return 
  *
  */

void COMLayer::write(byte val) { client_.write(val); }

/* 
  *   brief Write bytes to the server
  *  
  *   param buff - the buffer of bytes to be written, len - the number of bytes to write
  *   
  *   return 
  *
  */

void COMLayer::write(byte buff[], size_t len)
{
  client_.write(buff, len);
}
/* 
  *   brief Reads 1 byte from the  server
  *  
  *   aram 
  *   
  *   return the byte that was read from the server
  *
  */

byte COMLayer::read() { return client_.read(); }
/* 
  *   brief Reads bytes from the server and fills the buffer
  *  
  *   param buff - array of bytes to be filled with the data from the server, len - the number of bytes to read
  *   
  *   return the number of that 
  *
  */

uint32_t COMLayer::read(byte buff[], size_t len)
{
  return client_.read(buff, len);
}
/* 
  *   brief converts the ip from char array to byte array
  *  
  *   param ip_str - the ip as array of chars, ip_arr - the ip as array if bytes
  *   
  *   return 
  *
  */

void COMLayer::stringToIp(const char *ip_str, byte ip_arr[])
{
  size_t index = 0;

  while (*ip_str) {
      if (isdigit((unsigned char)*ip_str)) {
          ip_arr[index] *= 10;
          ip_arr[index] += *ip_str - '0';
      } else {
          index++;
      }
      ip_str++;
  }
}
