all: server client

server: server.c
	cc -o server server.c

client: client.c
	cc -o client client.c

clean:
	rm client
	rm server
