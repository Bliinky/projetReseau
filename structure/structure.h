#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "clientData/DonneeClient.h"
//1
struct protocoleEnvoiePort
{
  int proto;
  int port;
};

//2

struct protocoleRecupereClient
{
  int proto;
};
struct protocoleEnvoieDonnee
{
  int proto;
  int ip;
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

#endif




