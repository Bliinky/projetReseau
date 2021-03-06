#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
using namespace std;

#include "DonneeClient.h"
#include "TableauClient.h"

TableauClient::TableauClient(){
  verrou_init();
}

TableauClient::~TableauClient()
{
  for(vector<DonneeClient*>::iterator i = donnee.begin();
      i != donnee.end();
      ++i)
    {
      delete *i;
    }
}

vector<DonneeClient*> TableauClient::getDonnee()
{
  return donnee;
}

DonneeClient* TableauClient::getDonnee(unsigned short int rang)
{
  return donnee[rang];
}
pthread_mutex_t& TableauClient::getVerrou()
{
  return verrou;
}
void TableauClient::setDonnee(unsigned short int rang, DonneeClient* d)
{
  DonneeClient* copie = donnee[rang];
  donnee[rang] = d;
  delete copie;
}
void TableauClient::setVerrou(pthread_mutex_t verrou)
{
  this->verrou = verrou;
}
int TableauClient::size()
{
  return donnee.size();
}

void TableauClient::pushClient(DonneeClient* d)
{
  donnee.push_back(d);
}

void TableauClient::rmClient(unsigned short int rang)
{
  DonneeClient* copie = donnee[rang];
  donnee.erase(donnee.begin() + rang);
}
void TableauClient::verrou_init()
{
  if(pthread_mutex_init(&verrou,NULL) != 0)
    {
      perror("pthread_mutex_init");
      exit(EXIT_FAILURE);
    }
}
