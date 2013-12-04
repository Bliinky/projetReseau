#ifndef SERVEUR_H
#define SERVEUR_H

class Serveur
{
 private:
  TableauClient donneeClients;
  TableauThread donneeThreads;
  Sock* brPublic;
  int descBrPublic;

  
  /**
   * Attente de connexion de client
  */
  void lancer_serveur();
    /**
   * Creation du thread qui va écrire au nouveu client
   *\param desc Le descripteur du client
   * \return L'id du thread
   */
  pthread_t creationThreadClient(int,struct sockaddr_in&);
 public:
  /**
   * Constructeur permettant de créer la socket et un TableauClient contenant l'ensemble des informations des clients connue par le Serveur
   */
  Serveur();
  /**
     Destructeur du Serveur permettant de désallouer la socket et supprimer l'ensemble des informations des clients.
  */
  ~Serveur();
  
  //Assceseurs
  TableauClient getDonneeClient();
  TableauThread getDonneeThread();
  Sock* getSock();
  int getDescBrPublic();

  void setDonneeClient(TableauClient);
  void setDonneeThread(TableauThread);
  void setSock(Sock*);
  void setDescBrPublic(int);

  /**
   * Methode qui va lancer le serveur
  */
  void init_serveur();
  
};

  /**
   * Thread écoutant un client
   */

void* thread_client(void*);
void envoieInformationClients(DonneeClient*,struct DescTableauClient*);
void recuperationPortClient(DonneeClient*, struct DescTableauClient*);
void suppresionClient(DonneeClient*, struct DescTableauClient*,int);
#endif
