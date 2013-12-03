#include "sock.h"
#include "sockdist.h"
#include <fstream>
#include <stdlib.h>
#include "unistd.h"
#include "pthread.h"
#include "sys/types.h"
#include "client.h"

Client::Client()
{
  char port[6];
  fstream infoClientFile("infoClient.txt", fstream::in);
  infoClientFile.getline(port,6);

  setPort(atoi(port));

  sockPub = new Sock(SOCK_STREAM,(short)getPort(),0);

  if(sockPub->good()) setDescSockPub(sockPub->getsDesc());
  else
    {
      cout << "Problème initialisation de la socket publique" << endl;
      exit(1);
    }
  cout << "Création de la socket publique réussi" << endl;
}

Client::~Client()
{
  close(descSockPub);
  close(descSockServeur);

  delete sockPub;
  delete sockServeur;
  delete sockDistServeur;
}

void Client::lancerPortEcoute()
{
  if(pthread_create(&idThServ, NULL, threadPortEcoute, &descSockPub) != 0)
    {
      cout << "Erreur lors du lancement du port d'écoute" << endl;
      exit(1);
    }
}

void Client::fermeturePortEcoute()
{
  if(pthread_join(idThServ, NULL) != 0)
    {
      cout << "Warning: Erreur lors du pthread_join() du port d'écoute" << endl;
    }
  cout << "Fin d'execution du port d'écoute" << endl;
}
				 		   

void Client::connexionServeur()
{
  sockServeur = new Sock(SOCK_STREAM, 0);
  if(sockServeur->good()) setDescSockServeur(sockServeur->getsDesc());
  else
    {
      cout << "Probleme initialisation de la socket serveur" << endl;
      exit(1);
    }
  cout << "Création de la socket serveur réussi" << endl;
 
  char port[6];

  fstream infoServeurFile("infoServeur.txt", fstream::in);
  infoServeurFile.getline(adresse,255);
  infoServeurFile.getline(port,6);
  serveurPort = atoi(port);

  sockDistServeur = new SockDist(adresse, (short)serveurPort);
  adrSockPub = sockDistServeur->getAdrDist();
  lgAdrSockPub = sizeof(struct sockaddr_in);
  
  cout << "Tentative de connexion au serveur \"" << adresse << ":" << serveurPort << "\"..." << endl;

  int connexion = connect(getDescSockServeur(), (struct sockaddr *)adrSockPub, lgAdrSockPub);

  if(connexion != 0)
    {
      cout << "La connexion à " << adresse << ":" << serveurPort << " a échoué !" << endl;
      exit(1);
    }
  
  cout << "Connexion établie avec le serveur" << endl;  
  
  struct protocolePort monPort;
  monPort.proto = 1;
  monPort.port = numeroPort;
  
  write(descSockServeur,&monPort,sizeof(struct protocolePort));
}

void Client::setDescSockServeur(int desc)
{
  descSockServeur = desc;
}

int Client::getDescSockServeur()
{
  return descSockServeur;
}

void Client::setDescSockPub(int desc)
{
  descSockPub = desc;
}

int Client::getDescSockPub()
{
  return descSockPub;
}

void Client::setPort(int port)
{
  numeroPort = port;
}

int Client::getPort()
{
  return numeroPort;
}

///////////////////////// Fonction //////////////////////////

void *threadPortEcoute(void *par)
{
  int *descSockPub = (int *)par;
  vector<int> clients;
  
  if(listen(*descSockPub,10) != 0)
    {
      cout << "Erreur création de la file d'attente" << endl;
      exit(1);
    }
  cout << "Création de la file d'attente réussi" << endl;

  struct sockaddr_in sockCV;
  socklen_t lgSockCV = sizeof(struct sockaddr_in);
  int descSockCV;

  while(true)
    {
      if((descSockCV = accept(*descSockPub, (struct sockaddr *)&sockCV, &lgSockCV)) < 0)
	{
	  cout << "Problème accept() du port d'écoute" << endl;
	  exit(1);
	}
      cout << "Nouvelle connection" << endl;
      clients.push_back(descSockCV);
    }
  
  pthread_exit(NULL);
}

void *threadClient(void *par)
{
  
}
