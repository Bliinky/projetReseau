#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include "../client.h"

using namespace std;

class Interface
{
 private:
  Client* client;

 public :
  Interface();
  ~Interface();

  void run();

  void lancerPortEcoute();
  void fermeturePortEcoute();
  void connexionServeur();
  void deconnexionServeur();

  void rafraichirClient();

  void envoyerFichier();
  void recupererFichier(char[]);

};

#endif
