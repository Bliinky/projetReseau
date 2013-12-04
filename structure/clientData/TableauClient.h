#ifndef TABLEAUCLIENT_H
#define TABLEAUCLIENT_H


class TableauClient
{
 private:
  vector<DonneeClient*> donnee; 
  pthread_mutex_t verrou;
 public:
  //Constructeur
  /**
     Construis un vector qui va contenir l'ensemble des informations des clients
  */
  TableauClient();
  /**
     Détruit l'ensemble des informations connu des clients
  */
  ~TableauClient();

  //Assceseurs
  /**
   * Permet de récupérer les informations des clients 
   * \return Le vecteur des informations des clients
   */
  vector<DonneeClient*> getDonnee();
  /**
   * \param rang Le rang dans le vecteur du client recherché
   * \return Les informations du client recherché

   */
  pthread_mutex_t& getVerrou();
  DonneeClient* getDonnee(unsigned short int);
  /**
   * \param rang Le rang dans le vecteur du client a changé
   * \param nouveaClient Les informations client 
   * \return Change l'ancien client par le nouveau
   */
  void setDonnee(unsigned short int,DonneeClient*);
  void setVerrou(pthread_mutex_t);
  /**
   * \return La taille du vecteur
   */
  int size();
  /**
   * Ajotue un ensemble d'information d'un client
   * \param donneeClient Nouveau ensemble d'information d'un client
   */
  void pushClient(DonneeClient*);
  /**
   * Suprime un ensemble d'information d'un client
   * \param rang Place dans le vecteur à rajouter
   */
  void rmClient(unsigned short int);
  int rangClient(DonneeClient* donnee_client);
  void verrou_init();
};

#endif
