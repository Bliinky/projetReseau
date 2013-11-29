#include "sock.h"
#include "sockdist.h"
#include <fstream>
#include <stdlib.h>
#include "client.h"

Client::Client()
{
  std::fstream infoClientFile("infoClient.txt", std::fstream::in);
  char port[6];
  infoClientFile.getline(port,6);
  setPort(atoi(port));
  sockPub = new Sock(SOCK_STREAM,(short)getPort(),0);
}

Client::~Client()
{

}

void Client::setPort(int port)
{
  numeroPort = port;
}

int Client::getPort()
{
  return numeroPort;
}
