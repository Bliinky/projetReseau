#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
using namespace std;


#include "fichiers.h"




int main()
{
  char nom[256] = "tot.png";
  char nomFichier[256] = "fichiers/tot.png.dos";
  supFichier(nomFichier,nom);
}
