#include "sock.h"
#include "sockdist.h"
#include <fstream>
#include <stdlib.h>
#include "string.h"
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
      perror("ConstructeurClient");
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
      perror("LancerPortEcoute");
      exit(1);
    }
}

void Client::fermeturePortEcoute()
{
  pthread_cancel(idThServ);
  if(pthread_join(idThServ, NULL) != 0)
    {
      perror("FermeturePortEcoute");
    }
  cout << "Fin d'execution du port d'écoute" << endl;
}
				 		   

void Client::connexionServeur()
{
  sockServeur = new Sock(SOCK_STREAM, 0);
  if(sockServeur->good()) setDescSockServeur(sockServeur->getsDesc());
  else
    {
      perror("ConnexionServeur");
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
  
  struct protocoleEnvoiePort monPort;
  monPort.proto = 1;
  monPort.port = numeroPort;
  
  write(descSockServeur,&monPort,sizeof(struct protocoleEnvoiePort));

  if(pthread_create(&idThServPrin,NULL,threadReceptionServeurPrin, &descSockServeur) != 0)
    {
      perror("ConnexionServeur");
      exit(1);
    }
}

void Client::deconnexionServeur()
{
  close(descSockServeur);
  /*if(pthread_join(idThServPrin, NULL) != 0)
    {
      perror("Deconnexion du serveur");
      }*/
  cout << "Deconnexion du serveur" << endl;
}

void Client::envoyerFichier()
{
  
}

void Client::recupererFichier(char* nomFichier)
{
  struct protocoleRecupereFichier req;
  req.proto = 3;
  req.part = 69;
  strcpy(req.nom,nomFichier);
  req.taille = strlen(req.nom);

  write(descSockServeur,&req,sizeof(int)*3+req.taille);
}

void Client::rafraichirClient()
{
  struct protocoleRecupereClient prot;
  prot.proto = 2;

  write(descSockServeur,&prot,sizeof(prot));
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
      pthread_t *idThClient;
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

void *threadReceptionServeurPrin(void *par)
{
  int *descSockServeur = (int *)par;
  int proto;
  int fermeture;
  while((fermeture = read(*descSockServeur,&proto,4)) > 0)
    {
      switch(proto)
	{
	case 2:
	  int ip;
	  int port;
	  read(*descSockServeur,&ip,4);
	  read(*descSockServeur,&port,4);
	  
	  cout << ip << " " << port << endl;
	  break;
	}
    }
  if(fermeture == 0)
    {
      perror("ThreadReceptionServeurPrin");
    }
  if(fermeture == -1)
    {
      perror("ThreadReceptionServeurPrin");
    }
}
