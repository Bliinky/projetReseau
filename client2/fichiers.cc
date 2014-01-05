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
  strcpy(nomFichier,nom+pos);
  char* nomDossier = (char*)malloc(sizeof(char) * strlen(nom) + 5);
  strcpy(nomDossier,"fichiers/");
  strcat(nomDossier,nomFichier);
  strcat(nomDossier,".dos/");
  cout<<nomDossier<<endl;
  fstream f ;
  f.open(nom,fstream::in|fstream::out|fstream::app);
  supFichier(nomDossier);
  perror("rmdir");
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
	  fPartition->open(nom_partition,fstream::in|fstream::out|fstream::app);
	  perror("open");
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
  //cout<<"Je recupere "<<nom<<endl;
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
  /* cout<<"/////////////////////////////////////"<<endl;
  cout<<"voici n "<<n<<endl;
  cout<<"voici p "<<p<<" "<<strlen(p)<<endl;
  cout<<"voici partChar "<<partChar<<" "<<strlen(partChar)<<endl;
  cout<<"voici nom "<<nom<<" et taille "<<strlen(nom)<<endl;
  cout<<"/////////////////////////////////////"<<endl;*/
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
	  if(tok != NULL && strcmp(n,tok) == 0)
	    {
	      if(strstr(line,partChar) == NULL)
		{
		  cout<<"yolo"<<endl;
		  cout<<partChar<<endl;
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
	      strcat(line,"\n");
	      fCopy.write(line,strlen(line));
	      fCopy.flush();
	      //perror("write");
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
      char line[1000];
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
    }
  fCopy.flush();
  f.flush();
  f.close();
  fCopy.close();
  remove("fichiers/infoFichiers.txt");
  rename("fichiers/infoFichiersCopy.txt","fichiers/infoFichiers.txt");
  return 1;
}


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
  for(int i=0; i < taille; i++)
    {
      f_fichier.put(fichier[i]);
    }
  f_fichier.flush();
  f_fichier.close();
  //cout << "fin de reception partition" << endl;
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
  strcpy(n,nom);
  //n[strlen(nom) - strlen(p)] = '\0';

  fstream f(nom,fstream::out);
  int i = 0;
  fstream fPar;
  char nomDos[256];
  strcpy(nomDos,n);
  //strcpy(nomDos,"fichiers/");
  //strcat(nomDos,n);
  strcat(nomDos,".dos/");
  

  do
    {
      char nomPar[256];
      char iChar[10];
      sprintf(iChar,"%d",i);
      strcpy(nomPar,nomDos);
      strcat(nomPar,n);
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
      char line[1000];
      char* tok;
      f.getline(line,1000);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  cout<<n<<" "<<tok<<endl;
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
      char line[1000];
      char lineCopy[1000];
      char* tok;
      f.getline(line,1000);
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
  cout<<nomDos<<endl;
  DIR* dossier =  opendir(nomDos);
  if(dossier == NULL)
      return;
  struct dirent* fichier;
  vector<char*> v;
  while((fichier = readdir(dossier)) != NULL){
    char nomDosFichier[255];
    strcpy(nomDosFichier,nomDos);
    strcat(nomDosFichier,fichier->d_name);
    cout<<nomDosFichier<<endl;
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
  fstream f("fichiers/infoFichiers.txt",fstream::out|fstream::in);
  fstream fCopy("fichiers/infoFichiersCopy.txt", fstream::out);
  while(f.good())
    {
      char line[1000];
      char lineCopy[1000];
      char* tok;
      f.getline(line,1000);
      strcpy(lineCopy,line);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  if(tok!= NULL && !(strcmp(nom,tok) == 0))
	    {
	      strcat(lineCopy,"\n");
	      fCopy.write(lineCopy,strlen(lineCopy));
	    }
	}
    }
  char nbPartChar[10];
  sprintf(nbPartChar,"%d",nbPart);
  char line[1000];
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
      char line[1000];
      char* tok;
      f.getline(line,1000);
      if(f.good())
	{
	  tok = strtok(line,"=");
	  cout<<"Voici tok "<<tok<<endl;
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
