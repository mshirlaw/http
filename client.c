#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

int main(int argc, char **argv) {
	
	char *method;
	char *address;

	if (argc < 3) {
		printf("Usage: ./client <METHOD> <ADDRESS>\n");
		return 0;
	}

	method = argv[1];
	address = argv[2];

	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(80);
	remote_address.sin_addr.s_addr = inet_addr(address);

	connect(client_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));
	
	char *path = " / ";
	
	// ./client GET 216.58.199.35 for testing
	char *protocol = "HTTP/1.1\r\nHost: www.google.com.au\r\n\r\n";

	char request[1024];
	//char request[sizeof(method) + sizeof(path) + sizeof(protocol)];

	strcat(request, method);
	strcat(request, path);
	strcat(request, protocol);

	char response[4096];

	send(client_socket, request, sizeof(request), 0);
	recv(client_socket, &response, sizeof(response), 0);

	printf("%s\n", response);

	close(client_socket);

	return 0;
}

