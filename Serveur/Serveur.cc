#include <iostream>
#include <vector>
#include <string.h>
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



Serveur::Serveur()
{
  brPublic = new Sock(SOCK_STREAM, 0);
  if(brPublic->good()){descBrPublic = brPublic->getsDesc();}
  else{cout<<"Probleme initialisation de la socket"<<endl; 
    delete brPublic;
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
  int en_tete = -1;
  int isPresent;
  DonneeClient* donnee_client = NULL;
  while((isPresent = read(parametreClient->descClient,&en_tete,4) != 0 && isPresent != -1))
    {
      switch(en_tete)
	{
	case 1:
	  {
	    donnee_client = recuperationPortClient(parametreClient);
	    break;
	  }
	case 2:
	  {
	    envoieInformationClients(donnee_client,parametreClient);
	    break;
	  }
	case 3:
	  {
	    creationThreadPartition(parametreClient);
	    break;
	   
	  }
	}
    }
  suppresionClient(donnee_client,parametreClient,isPresent);
  pthread_exit(NULL);
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
	  protocoleDonnee_client.port = parametreClient->donneeClients->getDonnee(i)->getPort();
	  protocoleDonnee_client.ip = parametreClient->donneeClients->getDonnee(i)->getIp();
	  write(parametreClient->descClient,&protocoleDonnee_client,sizeof(protocoleEnvoieDonnee));
	  //cout<<"Le client envoyé a l'ip "<<protocoleDonnee_client.ip<< " et le port "<<protocoleDonnee_client.port<<endl;
	}
    }
    pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
}

DonneeClient* recuperationPortClient(struct DescTableauClient* parametreClient)
{
  cout<<"Récupération du port du client"<<endl;
  int numero_port;
  read(parametreClient->descClient,&numero_port,4);
  cout<<"Le numero port du client est: "<<numero_port<<endl;
  DonneeClient* donnee_client = new DonneeClient(parametreClient->adresse,numero_port,parametreClient->descClient);
  pthread_mutex_lock(&(parametreClient->donneeClients->getVerrou()));
  parametreClient->donneeClients->pushClient(donnee_client);
  pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
  return donnee_client;
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
  cout<<"rang : "<<rang<<endl;
  if(rang != -1)
    parametreClient->donneeClients->rmClient(rang);
  pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
  free(parametreClient);
}


 

//Protocole numero 3: Demande d'une partition


void creationThreadPartition(struct DescTableauClient* parametreClient)
{
  cout<<"Création thread_partition"<<endl;
  struct ParametreFichier * parametre_fichier = (struct ParametreFichier *)malloc(sizeof(struct ParametreFichier));
  pthread_t id;
  parametre_fichier->parametreClient = parametreClient;
  parametre_fichier->fichier.proto = 3;
  read(parametreClient->descClient,&parametre_fichier->fichier.part,4);
  read(parametreClient->descClient,&parametre_fichier->fichier.taille,4);
  read(parametreClient->descClient,parametre_fichier->fichier.nom ,parametre_fichier->fichier.taille);
  cout<<parametre_fichier->fichier.nom << " " << parametre_fichier->fichier.part << endl;
  if(pthread_create(&id,NULL,thread_partition,parametre_fichier))
    {
      cout<<"Erreur thread_partition creation "<<endl;
    }
}
	
void* thread_partition(void* par)
{
  struct ParametreFichier* parametre_fichier = (struct ParametreFichier*)par;
  pthread_mutex_lock(&(parametre_fichier->parametreClient->donneeClients->getVerrou()));
  for(int i = 0 ; i < parametre_fichier->parametreClient->donneeClients->size() ; i++)
    {
      int desc = parametre_fichier->parametreClient->donneeClients->getDonnee(i)->getDesc();
      if(desc==parametre_fichier->parametreClient->descClient)
	{
	  parametre_fichier->fichier.port = parametre_fichier->parametreClient->donneeClients->getDonnee(i)->getPort();
	  parametre_fichier->fichier.ip = parametre_fichier->parametreClient->donneeClients->getDonnee(i)->getIp();
	}
    }
  for(int i = 0 ; i < parametre_fichier->parametreClient->donneeClients->size() ; i++)
    {
      int desc = parametre_fichier->parametreClient->donneeClients->getDonnee(i)->getDesc();
      if(desc != parametre_fichier->parametreClient->descClient)
	if(-1 == write(desc,&(parametre_fichier->fichier),sizeof(int) * 4 + sizeof(in_addr) + parametre_fichier->fichier.taille))
	  cout<<"Erreur write"<<endl;
    }
  pthread_mutex_unlock(&(parametre_fichier->parametreClient->donneeClients->getVerrou()));
  free(parametre_fichier);
  pthread_exit(NULL);
}
