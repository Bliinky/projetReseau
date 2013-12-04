#include <iostream>
#include <vector>
#include <string.h>
#include <netinet/in.h>
using namespace std;

#include "DonneeClient.h"

DonneeClient::DonneeClient(){}
DonneeClient::DonneeClient(in_addr ip,int port)
{
  this->ip = ip;
  this->port = port;
}

DonneeClient::~DonneeClient()
{
}



in_addr DonneeClient::getIp()const
{
  return ip;
}
int DonneeClient::getPort()const
{
  return port;
}
void DonneeClient::setIp(in_addr ip)
{
  this->ip = ip;
}
void DonneeClient::setPort(int port)
{
  this->port = port;
}


DonneeClient& DonneeClient::operator=(const DonneeClient& client)
{
  this->ip = client.getIp();
  this->port = client.getPort();
  return *this;
}
