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



int decouperFichier(const char* nom, int N)
{
  char* nomDossier = (char*)malloc(sizeof(char) * strlen(nom) + 3);
  strcpy(nomDossier,nom);
  strcat(nomDossier,".dos");
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
  return partition;
}



int determineAction(char* nom, int part,int partTot)
{
   fstream fCopy;
  fCopy.open("fichiers/infoFichiersCopy.txt",fstream::out);
  perror("open");
  fstream f("fichiers/infoFichiers.txt",fstream::out|fstream::in);
  char p[255];
  sprintf(p,"%d",part);
  char partChar[255] = "\\";
  strcat(partChar,p);
  strcat(partChar,"\\");
  bool nomIsPresent = false;
  while(f.good())
    {
      char line[1000];
      char lineCopy[1000];
      char* tok;
      f.getline(line,1000);
      if(f.good())
	{
	  strcpy(lineCopy,line);
	  tok = strtok(lineCopy,"=");
	  if(strcmp(nom,tok) == 0)
	    {
	      if(strstr(line,partChar) == NULL)
		{
		  strcat(line,partChar+1);
		  fCopy.write(line,strlen(line));
		  nomIsPresent=true;
		}
	      else
		{
		  remove("fichiers/infoFichiersCopy");
		  return 0;
		}
	    }
	  else
	    {
	      strcat(line,"\n");
	      fCopy.write(line,strlen(line));
	      fCopy.flush();
	      perror("write");
	    }
	}
    }
  if(!nomIsPresent)
    {
      //Création dossier
      char dosNom[255];
      strcpy(dosNom,nom);
      strcat(dosNom,".dos");
      mkdir(dosNom,0666);

      //Création ligne infoFichiers
      char line[1000];
      char total[10];
      sprintf(total,"%d",partTot);
      strcpy(line,nom);
      strcat(line,"=");
      strcat(line,p);
      strcat(line,";");
      strcat(line,total);
      strcat(line,partChar);
      fCopy.write(line,strlen(line));
    }
  f.close();
  fCopy.close();
  remove("fichiers/infoFichiers.txt");
  rename("fichiers/infoFichiersCopy.txt","fichiers/infoFichiers.txt");
  return 1;
}


void ajouterPartition(char* nom,int part, char* fichier, int taille)
{
  char nomDos[255];
  strcpy(nomDos,nom);
  strcat(nomDos,".dos");
  
  fstream f_fichier;
  f_fichier.open(nomDos,fstream::out);
  if(f_fichier.bad()) cout<<"error ouverture"<<endl;
  for(int i=0; i < taille; i++)
    {
      f_fichier.put(fichier[i]);
    }
  f_fichier.close();
  cout << "fin de reception partition" << endl;
}
    
