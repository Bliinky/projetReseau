#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include "sock.h"
#include "sockdist.h"
#include "pthread.h"
#include "sys/types.h"
#include <vector>

using namespace std;

struct protocolePort
{
  int proto;
  int port;
};

class Client
{
 private :
  pthread_t idThServ;
  Sock *sockPub;
  int descSockPub;
  int numeroPort;

  Sock *sockServeur;
  int descSockServeur;
  char adresse[255];
  int serveurPort;
  SockDist *sockDistServeur;
  struct sockaddr_in *adrSockPub;
  int lgAdrSockPub;

 public :
  Client();
  ~Client();

  void lancerPortEcoute();
  void fermeturePortEcoute();
  void connexionServeur();

  int getDescSockServeur();
  void setDescSockServeur(int);
  int getDescSockPub();
  void setDescSockPub(int);
  void setPort(int port);
  int getPort();
  
};

void *threadPortEcoute(void *);
void *threadClient(void *);
#endif
