#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sys/msg.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <vector>
using namespace std;
#include "fichiers.h"


//ajouter rmdir
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
  strcpy(nomFichier,nom+pos+1);
  char* nomDossier = (char*)malloc(sizeof(char) * strlen(nomFichier) + 100);
  strcpy(nomDossier,"fichiers/");
  strcat(nomDossier,nomFichier);
  strcat(nomDossier,".dos/");
  
  fstream f ;
  f.open(nom,fstream::in|fstream::out|fstream::app);
  supFichier(nomDossier);
  //perror("rmdir");
  int dossierF = mkdir(nomDossier,0777);
  //perror("mkdir");
  fstream* fPartition = NULL; 
  int partition = 0;
  int parcoursNChar = 0;
  while(f.good())
    {
      if(parcoursNChar == 0)
	{
	  char* nom_partition =(char *)malloc(sizeof(char) * strlen(nom) * 2 + 200);
	  strcpy(nom_partition,nomDossier);
	  strcat(nom_partition,nomFichier);
	  char char_partition[20];
	  sprintf(char_partition,"%d",partition);
	  strcat(nom_partition,char_partition);
	  if(fPartition != NULL)
	    fPartition->close();
	  delete fPartition;
	  fPartition = new fstream();
	  fPartition->open(nom_partition,fstream::in|fstream::out|fstream::app);
	  //perror("open");
	  partition++;
	  free(nom_partition);
	}
      char c = f.get();
      if(f.good())
	{
	  fPartition->put(c);
	  parcoursNChar = (parcoursNChar + 1)% N;
	}
    }
  free(nomDossier);
  delete fPartition;
  //f.flush();
  f.close();
  return partition;
}



int determineAction(char* nom, int part,int partTot)
{

  FILE* f = fopen("fichiers/infoFichiers.txt","r+");
  FILE* fCopy = fopen("fichiers/infoFichiersCopy.txt","w+");
  char p[255];
  sprintf(p,"%d",part);
  char partChar[255] = "\\";
  strcat(partChar,p);
  strcat(partChar,"\\");
  bool nomIsPresent = false;
  char n[255];
  strcpy(n,nom);
  n[strlen(nom) - strlen(p)] = '\0';
  int w = 0;
  char c;
  char* line = (char*)malloc(sizeof(char)*2000);
  char* lineCopy = (char*)malloc(sizeof(char)*2000);
  while((c = fgetc(f)) != EOF)
    {
      strcpy(line,"");
      while(c!=EOF && c!= '\n')
	{
	  strcat(line,&c);
	  c = fgetc(f);
	}



      if(c!=EOF)
	{
	  strcpy(lineCopy,line);
	  char* tok;
	  tok = strtok(lineCopy,"=");
	  if(tok != NULL && strcmp(n,tok) == 0)
	    {
	      if(strstr(line,partChar) == NULL)
		{
		  
		  
		  char lineAdd[1000];
		  strcpy(lineAdd,n);
		  strcat(lineAdd,"=");
		  tok = strtok(NULL,";");
		  int nbPart = atoi(tok);
		  nbPart++;
		  char nbPartChar[10];
		  sprintf(nbPartChar,"%d",nbPart);
		  strcat(lineAdd,nbPartChar);
		  strcat(lineAdd,";");
		  strcat(lineAdd,line + suiteCharBuffer(line,';'));
		  strcat(lineAdd,partChar+1);
		  //strcat(lineAdd,"\\");
		  strcat(lineAdd,"\n");
		  fputs(lineAdd,fCopy);
		  nomIsPresent=true;
		  //	  fCopy.flush();
		}
	      else
		{
		  remove("fichiers/infoFichiersCopy");
		  return 0;
		}
	    }
	  else
	    {
	      if(tok != NULL)
		{
		  strcat(line,"\n");
		  fputs(line,fCopy);
		  //fCopy.flush();
		  //perror("write");
		}
	    }
	}
    }
  if(!nomIsPresent)
    {
      //Création dossier
      char dosNom[255];
      strcpy(dosNom,"fichiers/");
      strcat(dosNom,n);
      strcat(dosNom,".dos/");
      //cout<<"Création dossier "<<dosNom<<endl;
      mkdir(dosNom,0777);

      //Création ligne infoFichiers
      char total[10];
      sprintf(total,"%d",partTot);
      strcpy(line,n);
      strcat(line,"=");
      strcat(line,"1");
      strcat(line,";");
      strcat(line,total);
      strcat(line,partChar);
      strcat(line,"\n");
      fputs(line,fCopy);
      //fCopy.write(line,strlen(line));
      //free(line);
    }
  free(line);
  free(lineCopy);
  fclose(fCopy);
  fclose(f);
  //fCopy.flush();
  //f.flush();
  //f.close();
  //fCopy.close();
  remove("fichiers/infoFichiers.txt");
  rename("fichiers/infoFichiersCopy.txt","fichiers/infoFichiers.txt");
  return 1;
}

/*

int determineAction(char* nom, int part,int partTot)
{

   fstream fCopy;
  fCopy.open("fichiers/infoFichiersCopy.txt",fstream::out);
  //perror("open");
  fstream f("fichiers/infoFichiers.txt",fstream::out|fstream::in);
  char p[255];
  sprintf(p,"%d",part);
  char partChar[255] = "\\";
  strcat(partChar,p);
  strcat(partChar,"\\");
  bool nomIsPresent = false;
  char n[255];
  strcpy(n,nom);
  n[strlen(nom) - strlen(p)] = '\0';
  int w = 0;

  char c;
  char* line = (char*)malloc(sizeof(char)*2000);
  char* lineCopy = (char*)malloc(sizeof(char)*2000);
  while(f.good())
    {
      strcpy(line,"");
      cout<<w<<endl;
      if(f.good())
	c = f.get();
      w++;
      if(f.good())
	{
	  while(f.good() && c!= '\n')
	    {
	      if(f.good())
		strcat(line,&c);
	      c = f.get();

	     
	    }
	}



      if(f.good())
	{
	  strcpy(lineCopy,line);
	  char* tok;
	  tok = strtok(lineCopy,"=");
	  if(tok != NULL && strcmp(n,tok) == 0)
	    {
	      if(strstr(line,partChar) == NULL)
		{
		  
		  
		  char lineAdd[1000];
		  strcpy(lineAdd,n);
		  strcat(lineAdd,"=");
		  tok = strtok(NULL,";");
		  int nbPart = atoi(tok);
		  nbPart++;
		  char nbPartChar[10];
		  sprintf(nbPartChar,"%d",nbPart);
		  strcat(lineAdd,nbPartChar);
		  strcat(lineAdd,";");
		  strcat(lineAdd,line + suiteCharBuffer(line,';'));
		  strcat(lineAdd,partChar+1);
		  //strcat(lineAdd,"\\");
		  strcat(lineAdd,"\n");
		  fCopy.write(lineAdd,strlen(lineAdd));
		  nomIsPresent=true;
		  fCopy.flush();
		}
	      else
		{
		  remove("fichiers/infoFichiersCopy");
		  return 0;
		}
	    }
	  else
	    {
	      if(tok != NULL)
		{
		  strcat(line,"\n");
		  fCopy.write(line,strlen(line));
		  fCopy.flush();
		  //perror("write");
		}
	    }
	}
    }
  if(!nomIsPresent)
    {
      //Création dossier
      char dosNom[255];
      strcpy(dosNom,"fichiers/");
      strcat(dosNom,n);
      strcat(dosNom,".dos/");
      //cout<<"Création dossier "<<dosNom<<endl;
      mkdir(dosNom,0777);

      //Création ligne infoFichiers
      char total[10];
      sprintf(total,"%d",partTot);
      strcpy(line,n);
      strcat(line,"=");
      strcat(line,"1");
      strcat(line,";");
      strcat(line,total);
      strcat(line,partChar);
      strcat(line,"\n");
      fCopy.write(line,strlen(line));
      free(line);
    }
  free(line);
  free(lineCopy);
  fCopy.flush();
  f.flush();
  f.close();
  fCopy.close();
  remove("fichiers/infoFichiers.txt");
  rename("fichiers/infoFichiersCopy.txt","fichiers/infoFichiers.txt");
  return 1;
  }*/

void ajouterPartition(char* nom,int part, char* fichier, int taille)
{
  char p[10];
  sprintf(p,"%d",part);
  char n[255];
  strcpy(n,nom);
  n[strlen(nom) - strlen(p)] = '\0';
  char nomDos[255];
  strcpy(nomDos,"fichiers/");
  strcat(nomDos,n);
  strcat(nomDos,".dos/");
  strcat(nomDos,nom);
  fstream f_fichier;
  f_fichier.open(nomDos,fstream::out);
  //perror("open");
  if(f_fichier.bad()) cout<<"error ouverture"<<endl;
  else
    {
      for(int i=0; i < taille; i++)
	{
	  f_fichier.put(fichier[i]);
	}
      f_fichier.flush();
      f_fichier.close();
    }
  //cout << "fi reception partition" << endl;
}


int suiteCharBuffer(char* buffer, char sep)
{
  for(int i = 0 ; i < strlen(buffer) ; i++)
    {
      if(sep == buffer[i])
	{
	  return i+1;
	}
    }
} 


void regroupePartition(char* nom)
{
  // char p[255];
  //sprintf(p,"%d",part);
  char n[255];
  strcpy(n,"fichiers/");
  strcat(n,nom);
  //n[strlen(nom) - strlen(p)] = '\0';
  
  fstream f(n,fstream::out);
  if(f.fail())
    {
      perror("ouverture fichier regroupePartition");
      return;
    }
    
  int i = 0;
  fstream fPar;
  char nomDos[256];
  strcpy(nomDos,"fichiers/");
  strcat(nomDos,nom);
  //strcpy(nomDos,"fichiers/");
  //strcat(nomDos,n);
  strcat(nomDos,".dos/");
  
  do
    {
      char nomPar[256];
      char iChar[10];
      sprintf(iChar,"%d",i);
      strcpy(nomPar,nomDos);
      strcat(nomPar,nom);
      strcat(nomPar,iChar);
      fPar.open(nomPar,fstream::in);
      if(fPar.fail())
	{
	  f.flush();
	  //Sup dossier
	  f.close();
	  return;
	}
      else
	{
	  while(fPar.good())
	    {
	      char c = fPar.get();
	      if(fPar.good())
		f.put(c);
	    }
	}
      fPar.close();
      i++;
    }while(true);
}

vector<int> partitionManquante(char* nom)
{
  char n[255];
  strcpy(n,nom);
  vector<int> v;
  fstream f("fichiers/infoFichiers.txt",fstream::in);
  while(f.good())
    {
      char line[10000];
      char* tok;
      f.getline(line,10000);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  if(tok!= NULL && strcmp(n,tok) == 0)
	    {
	      int nbPartition;
	      int nbPartitionTot;
	      
	      tok = strtok(NULL,";");
	      nbPartition = atoi(tok);
	      tok = strtok(NULL,"\\");
	      nbPartitionTot = atoi(tok);
	      if(nbPartition >= nbPartitionTot)
		{
		  return v;
		}
	      else
		{
		  bool aPartition[nbPartitionTot];
		 for(int i = 0 ; i < nbPartitionTot + 1 ; i++)
		    {
		      aPartition[i] = false;
		    }
		  tok = strtok(NULL,"\\");
		  while(tok!=NULL)
		    {
		      int partition = atoi(tok);
		      aPartition[partition] = true;
		      tok = strtok(NULL,"\\");
		    }
		  for(int i = 0 ; i < nbPartitionTot + 1; i++)
		    {
		      if(aPartition[i] == false)
			{
			  v.push_back(i);
			}
		    }
		  return v;
		}
	    }
	}
    }
  

  return v;
}

bool aPartition(char* nom, int part)
{
  char p[255];
  sprintf(p,"%d",part);
  char partChar[255] = "\\";
  strcat(partChar,p);
  strcat(partChar,"\\");
  fstream f("fichiers/infoFichiers.txt",fstream::in);
  while(f.good())
    {
      char line[10000];
      char lineCopy[10000];
      char* tok;
      f.getline(line,10000);
      strcpy(lineCopy,line);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  if(tok!=NULL && strcmp(nom,tok) == 0)
	    {
	      if(strstr(lineCopy,partChar) != NULL)
		{
		  return true;
		}
	      else
		{
		  return false;
		}
	    }
	}
    }
  return false;
}
void supFichier(char* nomDos)
{
  /*int i = 0;
  char nomDosCopy[256];
  char nomConcatener[256];
  char part[10];
  fstream f;
  do{
    if(i != 0)
      {
	f.close();
	remove(nomDosCopy);
      }
    sprintf(part,"%d",i);
    strcpy(nomDosCopy,nomDos);
    strcat(nomDosCopy,"/");
    strcat(nomConcatener,nom);
    strcat(nomConcatener,part);
    strcat(nomDosCopy,nomConcatener);
    f.open(nomDosCopy,fstream::in);
    i++;
    }while(!f.fail());*/
  DIR* dossier =  opendir(nomDos);
  if(dossier == NULL)
      return;
  struct dirent* fichier;
  vector<char*> v;
  while((fichier = readdir(dossier)) != NULL){
    char nomDosFichier[255];
    strcpy(nomDosFichier,nomDos);
    strcat(nomDosFichier,fichier->d_name);
    remove(nomDosFichier);
  }
  closedir(dossier);
  /*for(int i = 0 ; i < v.size() ; i++)
    {
      cout<<v[i]<<endl;
      remove(v[i]);
      }*/
}

void decoupageAjout(char* nom, int nbPart)
{
  int nbPartCopy;
  fstream f("fichiers/infoFichiers.txt",fstream::out|fstream::in);
  fstream fCopy("fichiers/infoFichiersCopy.txt", fstream::out);
  while(f.good())
    {
      char line[10000];
      char lineCopy[10000];
      char* tok;
      f.getline(line,10000);
      strcpy(lineCopy,line);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  if(tok!= NULL && !(strcmp(nom,tok) == 0))
	    {
	      strcat(lineCopy,"\n");
	      fCopy.write(lineCopy,strlen(lineCopy));
	    }
	  else
	    {
	      if(tok != NULL)
		{
		  tok = strtok(NULL,";");
		  tok = strtok(NULL,"\\");
		  nbPartCopy = atoi(tok);
		}
	      
	    }
	}
    }
  char nbPartChar[10];
  if(nbPart == -1)
    {
      sprintf(nbPartChar,"%d",nbPartCopy);
    }
  else
    {
      sprintf(nbPartChar,"%d",nbPart);
    }
  char line[10000];
  strcpy(line,nom);
  strcat(line,"=0;");
  strcat(line,nbPartChar);
  strcat(line,"\\\n");
  fCopy.write(line,strlen(line));
  f.flush();
  fCopy.flush();
  f.close();
  fCopy.close();
  remove("fichiers/infoFichiers.txt");
  rename("fichiers/infoFichiersCopy.txt","fichiers/infoFichiers.txt");
}

void listerFichier()
{
  key_t cle = ftok("ftok",NOMBRE);
  int id=msgget(cle,IPC_CREAT|0666);
  
  if(id==-1){
    id=msgget(cle,0666);
  }
  struct nomFichier nom;
  nom.etiquette = 1;
  /*nom.nom[0] = 'a';
  msgsnd(id,&nom,sizeof(nomFichier)-sizeof(long),0);
  perror("msgsnd");*/
  fstream f("fichiers/infoFichiers.txt",fstream::out|fstream::in);
  while(f.good())
    {
      char line[10000];
      char* tok;
      f.getline(line,10000);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  strcpy(nom.nom,tok);
	  nom.nom[strlen(tok)] = '\0';
	  msgsnd(id,&nom,strlen(tok) + 1,0);
	  perror("msgsnd");
	}
    }
  f.close();
  /*while(true)
    {
      cout<<"yolo"<<endl;
      msgrcv(id,&nom,255,1,0);
      perror("msgRcv");
      cout<<"Voici le nom de la file "<<nom.nom<<endl;
      }*/
}


void ajouterPartitionIpc(char* nom, int part)
{
  key_t cle = ftok("ftok",NOMBRE);
  int id=msgget(cle,IPC_CREAT|0666);
  
  if(id==-1){
    id=msgget(cle,0666);
  }
  
  char p[10];
  sprintf(p,"%d",part);
  char n[255];
  strcpy(n,nom);
  n[strlen(nom) - strlen(p)] = '\0';
  
  struct ajouterPartition partit;
  partit.etiquette = 2;
  partit.part = part;
  strcpy(partit.nom,n);
  msgsnd(id,&partit,strlen(n)+4,0);
}
