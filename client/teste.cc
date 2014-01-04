#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;


#include "fichiers.h"




int main()
{
  char nom[526]="tot.png2";
  cout<<partitionManquante(nom,2)<<endl;
}
