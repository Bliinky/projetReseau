#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include "sock.h"
#include "sockdist.h"
#include "pthread.h"
#include "sys/types.h"
#include "../structure/structure.h"
#include <vector>

using namespace std;

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
  void deconnexionServeur();

  void envoyerFichier();
  void recupererFichier(char*);

  int getDescSockServeur();
  void setDescSockServeur(int);
  int getDescSockPub();
  void setDescSockPub(int);
  void setPort(int);
  int getPort();
  
};

void *threadPortEcoute(void *);
void *threadClient(void *);
#endif
