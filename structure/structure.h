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
  int nbPartition;
  int taille_nom;
  int taille_fichier;
  char n[2000];
};
//2

struct protocoleRecupereClient
{
  int proto;
};
struct protocoleEnvoieDonnee
{
  int proto;
  int port;
  in_addr ip;
};

//3
struct protocoleRecupereFichier
{
  int proto;
  int part;
  int taille;
  int port;
  in_addr ip;
  char nom[255];
};
struct protocoleRecupereFichier2
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

struct RecherchePartition
{
  int part;
  int taille;
  int port;
  in_addr ip;
  char nom[255];
};

#endif




