#include <iostream>
#include <vector>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#include "DonneeClient.h"

DonneeClient::DonneeClient(){}

DonneeClient::DonneeClient(in_addr ip,int desc)
{
  this->ip = ip;
  this->port = -1;
  this->desc = desc;
}
DonneeClient::DonneeClient(in_addr ip,int port,int desc)
{
  this->ip = ip;
  this->port = port;
  this->desc = desc;
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
int DonneeClient::getDesc()const{return desc;}
void DonneeClient::setIp(in_addr ip)
{
  this->ip = ip;
}
void DonneeClient::setPort(int port)
{
  this->port = port;
}
void DonneeClient::setDesc(int desc){this->desc = desc;}

DonneeClient& DonneeClient::operator=(const DonneeClient& client)
{
  this->ip = client.getIp();
  this->port = client.getPort();
  return *this;
}

bool DonneeClient::operator==(const DonneeClient& c)
{
  if(strcmp(inet_ntoa(this->ip), inet_ntoa(c.getIp())) == 0)
    return  this->port == c.getPort();
  else
    return false;
}

void DonneeClient::toString()
{
  cout<<"Port: "<<port<<" et Ip "<<inet_ntoa(ip)<<endl;
}
