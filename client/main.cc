#include <iostream>
#include "unistd.h"
#include "client.h"

using namespace std;

int main(int argc, char** argv)
{
  Client *c = new Client();
  c->lancerPortEcoute();
  c->connexionServeur();
  sleep(5);
  c->fermeturePortEcoute();
  delete c;
  return 0;
}
