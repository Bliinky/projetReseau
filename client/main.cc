#include <iostream>
#include "client.h"

using namespace std;

int main(int argc, char** argv)
{
  Client *c = new Client();
  cout << c->getPort() << endl;
  return 0;
}
