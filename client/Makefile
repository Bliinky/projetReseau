CC=g++
CFLAGS=-g

client: client.o main.o sock.o sockdist.o interface.o
	$(CC) -o client main.o client.o interface.o sock.o sockdist.o -lpthread

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

client.o: client.cc
	$(CC) $(CFLAGS) -c client.cc

interface.o: interface/interface.cc
	$(CC) $(CFLAGS) -c interface/interface.cc

sock.o: sock.cc
	$(CC) $(CFLAGS) -c sock.cc

sockdist.o: sockdist.cc
	$(CC) $(CFLAGS) -c sockdist.cc

clean:
	rm -rf *.o *~


