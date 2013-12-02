#ifndef TABLEAUTHREAD_H
#define TABLEAUTHREAD_H

class TableauThread
{
 private:
  vector<DonneeThread> donnee;
 public:
  TableauThread();
  ~TableauThread();

  vector<DonneeThread> getDonnee();
  DonneeThread getDonnee(int);
  void setDonnee(vector<DonneeThread>);
  void setDonnee(DonneeThread,int);

  void addDonneeThread(pthread_t,int);
 
  
};

#endif
