CC=g++
CFLAGS=-g



Serveur: principal.o Serveur.o DonneeClient.o TableauClient.o sock.o sockdist.o
	$(CC) -o Serveur principal.o Serveur.o DonneeClient.o TableauClient.o sock.o sockdist.o

principal.o: principal.cc
	$(CC) $(CFLAGS) -c principal.cc

Serveur.o: Serveur.cc
	$(CC) $(CFLAGS) -c Serveur.cc

DonneeClient.o: clientData/DonneeClient.cc
	$(CC) $(CFLAGS) -c clientData/DonneeClient.cc

TableauClient.o: clientData/TableauClient.cc
	$(CC) $(CFLAGS) -c clientData/TableauClient.cc

sock.o: sock.cc
	$(CC) $(CFLAGS) -c sock.cc

sockdist.o: sockdist.cc
	$(CC) $(CFLAGS) -c sockdist.cc


clean: 
	rm -rf *~ *.o
