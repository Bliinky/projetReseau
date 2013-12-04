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
     Cronstructeur vide
   */
  DonneeClient();
  /**
     Création d'un ensemble de donnée d'un client
  */
  DonneeClient(in_addr,int);
  /**
     Détruis l'ensemble des informations du client
  */
  ~DonneeClient();
  /**
     Operator =
   */
  DonneeClient& operator=(const DonneeClient&);

  //Assceseurs
  in_addr getIp()const;
  int getPort()const;
  char* getPortIp();
  void setIp(in_addr);
  void setPort(int);
};


#endif
