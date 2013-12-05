
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



void decoupeFichier(const char* nom, int N)
{
  char* nomDossier = (char*)malloc(sizeof(char) * strlen(nom) + 3);
  strcpy(nomDossier,nom);
  strcat(nomDossier,"dos/");
  fstream f ;
  f.open(nom,fstream::in|fstream::out|fstream::app);
  int dossierF = mkdir(nomDossier,0777);
  fstream* fPartition = NULL; 
  int partition = 0;
  int parcoursNChar = 0;
  while(f.good())
    {
      if(parcoursNChar == 0)
	{
	  char* nom_partition =(char *)malloc(sizeof(char) * strlen(nom) * 2 + 20);
	  strcpy(nom_partition,nomDossier);
	  strcat(nom_partition,nom);
	  char char_partition[20];
	  sprintf(char_partition,"%d",partition);
	  strcat(nom_partition,char_partition);
	  if(fPartition != NULL)
	    fPartition->close();
	  delete fPartition;
	  fPartition = new fstream();
	  fPartition->open(nom_partition,fstream::in|fstream::out|fstream::app);
	  partition++;
	  free(nom_partition);
	}
      fPartition->put(f.get());
      parcoursNChar = (parcoursNChar + 1)% N;
    }
  free(nomDossier);
  delete fPartition;
  f.close();
}

int main(int argc,char* argv[])
{
  cout<<argv[1]<<endl;
  decoupeFichier(argv[1],100);
}
