CC=g++
CFLAGS=-g



Serveur: DonneeThread.o TableauThread.o DonneeClient.o TableauClient.o sock.o sockdist.o principal.o Serveur.o 
	$(CC) -o Serveur principal.o Serveur.o DonneeClient.o TableauClient.o sock.o sockdist.o DonneeThread.o TableauThread.o -lpthread -g

DonneeThread.o: threadData/DonneeThread.cc
	$(CC) $(CFLAGS) -c threadData/DonneeThread.cc

TableauThread.o: threadData/TableauThread.cc
	$(CC) $(CFLAGS) -c threadData/TableauThread.cc

DonneeClient.o: ../structure/clientData/DonneeClient.cc
	$(CC) $(CFLAGS) -c ../structure/clientData/DonneeClient.cc

TableauClient.o: ../structure/clientData/TableauClient.cc 
	$(CC) $(CFLAGS) -c ../structure/clientData/TableauClient.cc 

principal.o: principal.cc
	$(CC) $(CFLAGS) -c principal.cc

Serveur.o: Serveur.cc
	$(CC) $(CFLAGS) -c Serveur.cc


sock.o: reseau/sock.cc
	$(CC) $(CFLAGS) -c reseau/sock.cc

sockdist.o: reseau/sockdist.cc
	$(CC) $(CFLAGS) -c reseau/sockdist.cc


clean: 
	rm -rf *~ *.o
