CC=g++
CFLAGS=-g

client: client.o main.o sock.o sockdist.o
	$(CC) -o client main.o client.o sock.o sockdist.o

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

client.o: client.cc
	$(CC) $(CFLAGS) -c client.cc

sock.o: sock.cc
	$(CC) $(CFLAGS) -c sock.cc

sockdist.o: sockdist.cc
	$(CC) $(CFLAGS) -c sockdist.cc

clean:
	rm -rf *.o *~


