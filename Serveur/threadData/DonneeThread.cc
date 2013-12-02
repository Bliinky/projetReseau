#include <iostream>
using namespace std;

#include "DonneeThread.h"

DonneeThread::DonneeThread(pthread_t t, int descClient)
{
  this->t = t;
  this->descClient = descClient;
}

DonneeThread::~DonneeThread(){}


pthread_t DonneeThread::getT()
{
  return t;
}
int DonneeThread::getDescClient()
{
  return descClient;
}
void DonneeThread::setT(pthread_t t)
{
  this->t = t;
}
void DonneeThread::setDescClient(short descClient)
{
  this->descClient = descClient;
}

