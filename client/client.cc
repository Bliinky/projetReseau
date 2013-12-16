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
  setDescSockServeur(-2); // Pour vérifier si le descripteur est utilisé

  char port[6];
  fstream infoClientFile("infoClient.txt", fstream::in);
  infoClientFile.getline(port,6);
  infoClientFile.getline(cheminFichiers,255);
  infoClientFile.close();

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
  if(descSockServeur != -2)
    {
      deconnexionServeur();
    }
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
  infoServeurFile.close();

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
  descSockServeur = -2;
  
  pthread_cancel(idThServPrin);
  if(pthread_join(idThServPrin, NULL) != 0)
    {
      perror("Deconnexion du serveur");
    }
  cout << "Deconnexion du serveur" << endl;
}

void Client::envoyerFichier(char* nomFichier)
{
  fstream infoClientFile("fichiers/infoFichiers.txt", fstream::in);
  char fileName[255];
  while(infoClientFile.getline(fileName,255))
    {
      char * tok;
      tok = strtok(fileName,"\"");
      tok = strtok(NULL,"\"");
      if(strcmp(tok,nomFichier) == 0)
	{ 
	  char cheminFichierEnvoi[255];
	  strcpy(cheminFichierEnvoi,cheminFichiers);
	  strcat(cheminFichierEnvoi,nomFichier);
	  cout << "Envoie du fichier : " << cheminFichierEnvoi << endl;
	  fstream fichierEnvoi(cheminFichierEnvoi, fstream::in);
	  if(!fichierEnvoi.good())
	    {
	      fichierEnvoi.close();
	      cout << "Erreur ouverture du fichier a envoyer" << endl;
	    }
	  else
	    {
	      fichierEnvoi.seekg(0,fichierEnvoi.end);
	      int taille = fichierEnvoi.tellg();
	      fichierEnvoi.seekg(0,fichierEnvoi.beg);
	      
	      char * buffer = new char [taille];
	      
	      fichierEnvoi.read (buffer,taille);

	      
	      struct p p1;
	      p1.proto = 1;
	      p1.part = 2;
	      p1.taille_nom = sizeof(nomFichier);
	      p1.taille_fichier = taille;
	      strcpy(p1.n,buffer);

	    }	  
	}
      else
	{
	  cout << "Fichier introuvable" << endl;
	}
    }
  
}

void Client::recupererFichier(char* nomFichier)
{
  if(descSockServeur == -2)
    {
      cout << "Vous n'etes pas connecté au serveur" << endl;
    }
  else
    {
      struct protocoleRecupereFichier req;
      req.proto = 3;
      req.part = 69;
      strcpy(req.nom,nomFichier);
      req.taille = strlen(req.nom);
      
      write(descSockServeur,&req,sizeof(int)*3+req.taille);
    }
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
  //TableauClient clients;
  
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
	  perror("accept");
	}
      else
	{
	  cout << "Nouvelle connection" << endl;
	  /* (*idThClient) = creationThreadClient(descSockCv, sockCV);
	  if((*idThClient) == -1)
	    {
	      close(descSockCV);
	    }
	  */
	}
      
      
    }
  
  pthread_exit(NULL);
}
/*
void *threadClient(void *par)
{
  struct DescTableauClient* parametreClient = (struct DescTableauCLient*)par;
  int en_tete = -1;
  int isPresent;
  DonneeClient* donnee_client = new DonneeClient(parametreClient->adresse,numero_port,parametreClient->descClient);
  pthread_mutex_lock(&(parametreClient->donneeClients->getVerrou()));
  parametreClient->donneeClient->pushClient(donnee_client);
  pthread_mutex_unlock(&(parametreClient->donneeClients->getVerrou()));
  while((isPresent = read(parametreClient->descClient, &en_tete,4) != 0 && isPresent != -1))
    {
      switch(en_tete)
	{
	case 1: 
	  {
	    int idFichier;
	    read(parametreClient->descClient,&idFichier,4);
	    int taille;
	    read(parametreClient->descClient,&taille,4);
	    char nom = (char*)malloc(sizeof(char) * taille);
	    
	    break;
	  }
	}
    }
  suppresionClient(donnee_client,parametreClient,isPresent);
  pthread_exit(par);
}

*/

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
/*
pthread_t Client::creationThreadClient(int descBrCircuitVirtuel,struct sockaddr_in& adresse)
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
}*/
