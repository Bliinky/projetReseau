#ifndef DONNEECLIENT_H
#define DONNECLIENT_H

class DonneeClient
{
 private:
  in_addr ip;
  int port;
 public:
  //Constructeur
  /**
     Création d'un ensemble de donnée d'un client
  */
  DonneeClient(in_addr,int);
  /**
     Détruis l'ensemble des informations du client
  */
  ~DonneeClient();
  
  //Assceseurs
  in_addr getIp();
  int getPort();
  char* getPortIp();
  void setIp(in_addr);
  void setPort(int);
};


#endif
