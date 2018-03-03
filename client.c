#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <unistd.h>

char *bldreq(const char *method, const char *path, const char *name) {
	char *header = "HTTP/1.1\r\nHost: ";
	char *term = "\r\n\r\n";

	size_t size = strlen(method) 
		+ strlen(path) 
		+ strlen(header) 
		+ strlen(name) 
		+ strlen(term) + 1;

	char *request = malloc(size);
	strcpy(request, method);
	strcat(request, path);
	strcat(request, header);
	strcat(request, name);
	strcat(request, term);

	return request;
}

int main(int argc, char **argv) {
	
	char *method;
	char *name;

	if (argc < 3) {
		printf("Usage: ./client <METHOD> <HOST NAME>\n");
		return 0;
	}

	method = argv[1];
	name = argv[2];

	struct hostent *host;
	host = gethostbyname(name);

	struct servent *srv;
	srv = getservbyname("http", "tcp");

	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = srv->s_port;	
	memcpy(&remote_address.sin_addr, host->h_addr_list[0], host->h_length);

	connect(client_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));

	char *request = bldreq(method, " / ", host->h_name);
	char response[4096];

	send(client_socket, request, strlen(request), 0);
	recv(client_socket, &response, sizeof(response), 0);

	printf("%s\n", response);

	free(request);
	close(client_socket);

	return 0;
}

