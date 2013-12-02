#ifndef DONNEETHREAD_H
#define DONNEETHREAD_H

class DonneeThread
{
 private:
  pthread_t t;
  int descClient;
 public:
  DonneeThread(pthread_t, int);
  ~DonneeThread();

  //Assceseurs
  pthread_t getT();
  int getDescClient();
  void setT(pthread_t);
  void setDescClient(short);
};

#endif
