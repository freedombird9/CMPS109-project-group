# Makefile for the socket programming example
#

CC = g++
CFLAGS =-g -std=c++11 -Wall -O0
simple_server_objects = ServerSocket.o Socket.o simple_server_main.o hex.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o hex.o


all : simple_server simple_client

simple_server: $(simple_server_objects)
	$(CC) $(CFLAGS) -o simple_server $(simple_server_objects)


simple_client: $(simple_client_objects)
	$(CC) $(CFLAGS) -o simple_client $(simple_client_objects)

hex.o: hex.cpp 
	$(CC) $(CFLAGS) -c hex.cpp 

Socket.o: Socket.cpp 
	$(CC) $(CFLAGS) -c Socket.cpp 

ServerSocket.o:  ServerSocket.cpp
	$(CC) $(CFLAGS) -c ServerSocket.cpp 


ClientSocket.o: ClientSocket.cpp 
	$(CC) $(CFLAGS) -c ClientSocket.cpp 

simple_server_main.o: simple_server_main.cpp
	$(CC) $(CFLAGS) -c simple_server_main.cpp

simple_client_main.o: simple_client_main.cpp
	$(CC) $(CFLAGS) -c simple_client_main.cpp

clean:
	rm -f *.o simple_server simple_client