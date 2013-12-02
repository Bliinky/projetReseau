#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

#include "DonneeClient.h"

DonneeClient::DonneeClient(char* ip,short port)
{
  this->ip = new char[strlen(ip)];
  this->port = port;
}

DonneeClient::~DonneeClient()
{
  delete[] ip;
}

char* DonneeClient::getIp()
{
  return ip;
}
short DonneeClient::getPort()
{
  return port;
}
void DonneeClient::setIp(char* ip)
{
  delete ip;
  this->ip = ip;
}
void DonneeClient::setPort(short port)
{
  this->port = port;
}
