#include <iostream>
#include "unistd.h"
#include "interface/interface.h"

using namespace std;

int main(int argc, char** argv)
{
  Interface *i = new Interface();
  i->run();
  delete i;
  return 0;
}
