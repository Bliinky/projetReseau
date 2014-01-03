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


int decouperFichier(const char* nom, int N)
{
  char nomFichier[255];
  int pos=0;
  for(int k = 0 ; k < strlen(nom) ; k++)
    {
      if(nom[k] == '/')
	{
	  pos=k;
	}
    }
  strcpy(nomFichier,nom+pos);
  char* nomDossier = (char*)malloc(sizeof(char) * strlen(nom) + 5);
  strcpy(nomDossier,nom);
  strcat(nomDossier,".dos/");
  cout<<nomDossier<<endl;
  fstream f ;
  f.open(nom,fstream::in|fstream::out|fstream::app);
  //rmdir(nomDossier);
  int dossierF = mkdir(nomDossier,0777);
  perror("mkdir");
  fstream* fPartition = NULL; 
  int partition = 0;
  int parcoursNChar = 0;
  while(f.good())
    {
      if(parcoursNChar == 0)
	{
	  char* nom_partition =(char *)malloc(sizeof(char) * strlen(nom) * 2 + 20);
	  strcpy(nom_partition,nomDossier);
	  strcat(nom_partition,nomFichier);
	  char char_partition[20];
	  sprintf(char_partition,"%d",partition);
	  strcat(nom_partition,char_partition);
	  if(fPartition != NULL)
	    fPartition->close();
	  delete fPartition;
	  fPartition = new fstream();
	  cout<<nom_partition<<endl;
	  fPartition->open(nom_partition,fstream::out|fstream::app);
	  perror("open");
	  partition++;
	  free(nom_partition);
	}
      fPartition->put(f.get());
      parcoursNChar = (parcoursNChar + 1)% N;
    }
  free(nomDossier);
  delete fPartition;
  f.close();
  return partition;
}

int main()
{
  decouperFichier("fichiers/toto.png",100);
}
