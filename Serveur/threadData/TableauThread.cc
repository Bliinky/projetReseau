#include <iostream>
#include <vector>
using namespace std;

#include "DonneeThread.h"
#include "TableauThread.h"

TableauThread::TableauThread(){}
TableauThread::~TableauThread(){}

vector<DonneeThread> TableauThread::getDonnee(){return donnee;}
DonneeThread TableauThread::getDonnee(int rang){return donnee[rang];}
void TableauThread::setDonnee(vector<DonneeThread> donnee){this->donnee = donnee;}
void TableauThread::setDonnee(DonneeThread d,int rang){donnee[rang] = d;}

void TableauThread::addDonneeThread(pthread_t t, int desc)
{
  DonneeThread add(t,desc);
  donnee.push_back(add);
}
