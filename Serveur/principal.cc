#include <iostream>
#include <vector>
using namespace std;

#include "reseau/sock.h"
#include "reseau/sockdist.h"
#include "threadData/DonneeThread.h"
#include "threadData/TableauThread.h"
#include "../structure/clientData/DonneeClient.h"
#include "../structure/clientData/TableauClient.h"
#include "Serveur.h"
#include "../structure/structure.h"
int main()
{
  Serveur* s = new Serveur();
  s->init_serveur();
  delete s;
  return 0;
}
