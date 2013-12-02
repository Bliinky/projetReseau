#ifndef DONNEECLIENT_H
#define DONNECLIENT_H

class DonneeClient
{
 private:
  char* ip;
  short port;
 public:
  //Constructeur
  /**
     Création d'un ensemble de donnée d'un client
  */
  DonneeClient(char*,short);
  /**
     Détruis l'ensemble des informations du client
  */
  ~DonneeClient();
  
  //Assceseurs
  char* getIp();
  short getPort();
  char* getPortIp();
  void setIp(char*);
  void setPort(short);
};


#endif
