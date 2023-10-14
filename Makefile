all: server client

runserver: server
	./server

runclient: client
	./client

server: ./src/server.c
	gcc ./src/server.c -o server

client: ./src/client.c
	gcc ./src/client.c -o client

clean:
	rm server client *.zip
	
zip:
	zip trab1 *.c Makefile 