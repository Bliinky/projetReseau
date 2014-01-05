CC=g++
CFLAGS=-g

client: client.o main.o sock.o sockdist.o interface.o DonneeClient.o TableauClient.o fichiers.o
	$(CC) -o client main.o client.o interface.o sock.o sockdist.o DonneeClient.o TableauClient.o fichiers.o -lpthread

fichiers.o: fichiers.cc
	$(CC) $(CFLAGS) -c fichiers.cc

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

DonneeClient.o : ../structure/clientData/DonneeClient.cc
	$(CC) $(CFLAGS) -c ../structure/clientData/DonneeClient.cc

TableauClient.o :../structure/clientData/TableauClient.cc
	$(CC) $(CFLAGS) -c ../structure/clientData/TableauClient.cc

client.o: client.cc
	$(CC) $(CFLAGS) -c client.cc

interface.o: interface/interface.cc
	$(CC) $(CFLAGS) -c interface/interface.cc

sock.o: sock.cc
	$(CC) $(CFLAGS) -c sock.cc

sockdist.o: sockdist.cc
	$(CC) $(CFLAGS) -c sockdist.cc

clean:
	rm -rf *.o *~ client


