#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include "sock.h"
#include "sockdist.h"

using namespace std;

class Client
{
 private :
  Sock *sockPub;
  int numeroPort;

 public :
  Client();
  ~Client();

  void setPort(int port);
  int getPort();
  
};
#endif
