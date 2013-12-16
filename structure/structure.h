#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "clientData/DonneeClient.h"
#include "clientData/TableauClient.h"
//1
struct protocoleEnvoiePort
{
  int proto;
  int port;
};
struct protocoleEnvoieFichier
{
  int proto;
  int part;
  int taille_nom;
  int taille_fichier;
  char nom_et_fichier[60000];
};
//2

struct protocoleRecupereClient
{
  int proto;
};
struct protocoleEnvoieDonnee
{
  int proto;
  in_addr ip;
  int port;
};

//3
struct protocoleRecupereFichier
{
  int proto;
  int part;
  int taille;
  char nom[255];
};

struct tableauDescServeur
{
  TableauClient* donneeClients;
  int descServeur;
};

struct DescTableauClient
{
  TableauClient* donneeClients;
  int descClient;
  in_addr adresse;
  
};

struct ParametreFichier
{
  struct DescTableauClient* parametreClient;
  struct protocoleRecupereFichier fichier;
};

#endif




