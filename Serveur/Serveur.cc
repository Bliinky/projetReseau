#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
using namespace std;

#include "reseau/sock.h"
#include "reseau/sockdist.h"
#include "clientData/DonneeClient.h"
#include "clientData/TableauClient.h"
#include "threadData/DonneeThread.h"
#include "threadData/TableauThread.h"
#include "Serveur.h"

struct DescTableauClient
{
  TableauClient donneeClients;
  int descClient;
};

Serveur::Serveur()
{
  brPublic = new Sock(SOCK_STREAM, 0);
  if(brPublic->good()){descBrPublic = brPublic->getsDesc();}
  else{cout<<"Probleme initialisation de la socket"<<endl; 
    exit(-1);}
  listen(descBrPublic,100);
}
Serveur::~Serveur()
{
  delete brPublic;
}

TableauClient Serveur::getDonneeClient()
{
  return donneeClients;
}
TableauThread Serveur::getDonneeThread()
{
  return donneeThreads;
}
Sock* Serveur::getSock()
{
  return brPublic;
}
int Serveur::getDescBrPublic()
{
  return descBrPublic;
}
void Serveur::setDonneeClient(TableauClient donneeClient)
{
  this->donneeClients = donneeClient;
}
void Serveur::setDonneeThread(TableauThread donneeThread)
{
  this->donneeThreads = donneeThread;
}
void Serveur::setSock(Sock * brPublic)
{
  this->brPublic = brPublic;
}
void Serveur::setDescBrPublic(int descBrPublic)
{
  this->descBrPublic = descBrPublic;
}
void Serveur::init_serveur()
{
  lancer_serveur();
}

void Serveur::lancer_serveur()
{
  while(true)
    {
      struct sockaddr_in brCircuitVirtuel;
      socklen_t lgrBrCircuitVirtuel;
      int descBrCircuitVirtuel = accept(descBrPublic,(sockaddr *)&brCircuitVirtuel,&lgrBrCircuitVirtuel);
      if(descBrCircuitVirtuel == -1)
	{
	  cout<<"Problème accept"<<endl;
	}
      else
	{
	  pthread_t id;
	  id = creationThreadClient(descBrCircuitVirtuel);
	  if(id != -1)
	    {
	      donneeThreads.addDonneeThread(id,descBrCircuitVirtuel);
	    }
	  else
	    {
	      close(descBrCircuitVirtuel);
	    }
     	}
    }
}


pthread_t Serveur::creationThreadClient(int descBrCircuitVirtuel)
{
  pthread_t id;
  struct DescTableauClient parametreThread;
  parametreThread.donneeClients = donneeClients;
  parametreThread.descClient = descBrCircuitVirtuel;
  if(pthread_create(&id,NULL,thread_client,&parametreThread) != 0)
    {
      cout<<"Erreut thread creation"<<endl;
      return -1;
    }
  else
    {
      return id;
    }
}





//////////////////////////////FONCTI0N/////////////////////////////////////////

void* thread_client(void* par)
{
  struct DescTableauClient* parametreClient = (struct DescTableauClient*)par;
  int en_tete = -1;
  int isPresent;
  while((isPresent = read(parametreClient->descClient,&en_tete,4) != 0 && isPresent != -1))
    {
      switch(en_tete)
	{
	case 1:
	    {
	      cout<<"def protocole"<<endl;
	    }
	}
    }
    if(isPresent == -1)
      perror("read");
    close(parametreClient->descClient);
    pthread_exit(par);
}
