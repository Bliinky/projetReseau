#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <vector>
#include "sock.h"
#include "sockdist.h"
#include "pthread.h"
#include "sys/types.h"
#include "../structure/clientData/DonneeClient.h"
#include "../structure/clientData/TableauClient.h"
#include "fichiers.h"

using namespace std;

struct envoieFichier
{
  char nomFichier[255];
  TableauClient *donneeClients;
  struct sockaddr_in *adrSockPub;
  int lgAdrSockPub;
};

class Client
{
 private :
  pthread_t idThServ;
  Sock *sockPub;
  int descSockPub;
  int numeroPort;
  TableauClient donneeClients;
  
  pthread_t idThServPrin;
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

  void envoyerFichier(char*);
  void recupererFichier(char*);
  void rafraichirClient();

  int getDescSockServeur();
  void setDescSockServeur(int);
  int getDescSockPub();
  void setDescSockPub(int);
  void setPort(int);
  int getPort();
  
};

void *threadPortEcoute(void *);
void *threadEnvoyerFichier(void *);
void *threadClient(void *);
void *threadReceptionServeurPrin(void *par);
void portIpClient(TableauClient*, int);
pthread_t* creationThreadClient(TableauClient*, int, struct sockaddr_in&);
void suppresionClient(DonneeClient*, struct DescTableauClient*, int);
void recuperationPartition(int);
void ecriturePartition(int,char*,char*,int,int);
int choixAction();
void recherchePartition(int);
void *threadRecherchePartition(void*);
void receptionPartitionManquante(int);

#endif
