#ifndef FICHIERS_H
#define FICHIERS_H
#define NOMBRE 10

//Struct pour thread et affichade dans qt
struct nomFichier
{
  long etiquette;
  char nom[255];
};
struct ajouterPartition
{
  long etiquette;
  int part;
  char nom[255];
};


int determineAction(char*, int,int);
int decouperFichier(const char*,int);
void ajouterPartition(char*,int,char*,int);
int suiteCharBuffer(char* buffer,char sep);
void regroupePartition(char*);
vector<int> partitionManquante(char*);
bool aPartition(char*, int);
void supFichier(char*);
void decoupageAjout(char*,int);

void ajouterPartitionIpc(char*,int);
void listerFichier();
#endif
