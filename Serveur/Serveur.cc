#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
using namespace std;
#include "../structure/structure.h"
#include "reseau/sock.h"
#include "reseau/sockdist.h"
#include "../structure/clientData/DonneeClient.h"
#include "../structure/clientData/TableauClient.h"
#include "threadData/DonneeThread.h"
#include "threadData/TableauThread.h"
#include "Serveur.h"

struct DescTableauClient
{
  TableauClient* donneeClients;
  int descClient;
  in_addr adresse;
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
  for(int i = 0 ; i < donneeClients.size() ; i++)
    {
      close(donneeClients.getDonnee(i)->getDesc());
    }
  close(descBrPublic);
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
  cout<<"Lancement Serveur"<<endl;
  while(true)
    {
      struct sockaddr_in brCircuitVirtuel;
      socklen_t lgrBrCircuitVirtuel = sizeof(struct sockaddr_in);
      int descBrCircuitVirtuel = accept(descBrPublic,(sockaddr *)&brCircuitVirtuel,&lgrBrCircuitVirtuel);
      if(descBrCircuitVirtuel == -1)
	{
	  perror("accept");
	  cout<<"Problème accept"<<endl;
	}
      else
	{
	  pthread_t id;
	  id = creationThreadClient(descBrCircuitVirtuel,brCircuitVirtuel);
	  if(id != -1)
	    {
	      cout<<"Voici l'id du client "<<descBrCircuitVirtuel<<endl;
	      donneeThreads.addDonneeThread(id,descBrCircuitVirtuel);
	    }
	  else
	    {
	      close(descBrCircuitVirtuel);
	    }
     	}
    }
}


pthread_t Serveur::creationThreadClient(int descBrCircuitVirtuel,struct sockaddr_in& adresse)
{
  pthread_t id;
  struct DescTableauClient* parametreThread = (struct DescTableauClient*)malloc(sizeof(struct DescTableauClient));
  parametreThread->donneeClients = &donneeClients;
  parametreThread->descClient = descBrCircuitVirtuel;
  parametreThread->adresse = adresse.sin_addr;
  if(pthread_create(&id,NULL,thread_client,(void*)parametreThread) != 0)
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
  cout<<parametreClient->descClient<<endl;
  int en_tete = -1;
  int isPresent;
  DonneeClient* donnee_client = NULL;
  while((isPresent = read(parametreClient->descClient,&en_tete,4) != 0 && isPresent != -1))
    {
      switch(en_tete)
	{
	case 1:
	  {
	    recuperationPortClient(donnee_client,parametreClient);
	    break;
	  }
	case 2:
	  {
	    envoieInformationClients(donnee_client,parametreClient);
	    break;
	  }
	case 3:
	  {
	    if(pthread_create(&id,NULL,thread_recupere_fichier,(void*)
	    int part = 0;
	    read(parametreClient->descClient,&part,4);
	    cout<<part<<endl;
	    int taille = 0;
	    read(parametreClient->descClient,&taille,4);
	    cout<<taille<<endl;
	    char* nom_fichier = (char*) malloc(sizeof(char) * taille);
	    read(parametreClient->descClient,nom_fichier,taille);
	    cout<<nom_fichier<<endl;
	  }
	}
    }
  suppresionClient(donnee_client,parametreClient,isPresent);
  pthread_exit(par);
}



void envoieInformationClients(DonneeClient* donnee_client,struct DescTableauClient* parametreClient)
{
  cout<<"Envoie les informations des autres clients à notre client traitré dans le thread"<<endl;
  pthread_mutex_lock(&(parametreClient->donneeClients->getVerrou()));
  for(int i = 0 ; i < parametreClient->donneeClients->size() ; i++)
     {
      if(parametreClient->donneeClients->getDonnee(i) != donnee_client)
	{
	  struct protocoleEnvoieDonnee protocoleDonnee_client;
	  protocoleDonnee_client.proto = 2;
	  protocoleDonnee_client.donnee = *(parametreClient->donneeClients->getDonnee(i));
	  write(parametreClient->descClient,&protocoleDonnee_client,sizeof(protocoleDonnee_client));
	}
    }
}

void recuperationPortClient(DonneeClient* donnee_client,struct DescTableauClient* parametreClient)
{
  cout<<"Récupération du port du client"<<endl;
  int numero_port;
  read(parametreClient->descClient,&numero_port,4);
  cout<<"Le numero port du client est: "<<numero_port<<endl;
  donnee_client = new DonneeClient(parametreClient->adresse,numero_port,parametreClient->descClient);
  pthread_mutex_lock(&(parametreClient->donneeClients->getVerrou()));
  parametreClient->donneeClients->pushClient(donnee_client);
  pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
}

void suppresionClient(DonneeClient* donnee_client,struct DescTableauClient* parametreClient,int isPresent)
{
  cout<<"Suppresion client"<<endl;
  if(isPresent == -1)
    perror("read");
  close(parametreClient->descClient);
  pthread_mutex_lock(&(parametreClient->donneeClients->getVerrou()));
  int rang = -1;
  if(donnee_client != NULL)
    rang =  parametreClient->donneeClients->rangClient(donnee_client); 
  if(rang != -1)
    parametreClient->donneeClients->rmClient(rang);
  pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
  free(parametreClient);
}
