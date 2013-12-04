#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "clientData/DonneeClient.h"
struct protocoleEnvoiePort
{
  int proto;
  int port;
};


struct protocoleEnvoieDonnee
{
  int proto;
  DonneeClient donnee;
};

struct protocoleRecupereFichier
{
  int proto;
  int part;
  int taille;
  char nom[255];
};

#endif




