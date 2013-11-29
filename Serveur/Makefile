CC=g++
CFLAGS=-g

Serveur: principal.o Serveur.o donneeClient.o tableauClient.o sock.o sockdist.o
	$(CC) -o Serveur principal.o Serveur.o donneeClient.o tableauClient.o sock.o sockdist.o

principal.o: principal.cc
	$(CC) $(CFLAGS) -c principal.cc

Serveur.o: Serveur.cc
	$(CC) $(CFLAGS) -c Serveur.cc

client/donneeClient.o: client/donneeClient.cc
	$(CC) $(CFLAGS) -c client/donneeClient.cc

client/tableauClient.o: client/tableauClient.cc
	$(CC) $(CFLAGS) -c client/tableauClient.cc

sock.o: sock.cc
	$(CC) $(CFLAGS) -c sock.cc

sockdist.o: sockdist.cc
	$(CC) $(CFLAGS) -c sockdist.cc


