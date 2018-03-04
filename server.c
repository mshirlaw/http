#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	FILE *html;
	html = fopen("index.html", "r");

	fseek(html, 0, SEEK_END);
	int file_size = ftell(html);
	fseek(html, 0, SEEK_SET);

	char html_data[file_size];
	bzero(html_data, file_size);

	fgets(html_data, file_size, html);

	char http_header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\n";

	char http_response[sizeof(http_header) + file_size];
	bzero(http_response, sizeof(http_header) + file_size);
	
	strcpy(http_response, http_header);
	strcat(http_response, html_data);

	int server_socket, client_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		fprintf(stderr, "Error, unable to open socket\n");
		exit(0);
	}

	struct sockaddr_in server_address;
	bzero((char *) &server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int ret;
	ret = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	if (ret < 0) {
		fprintf(stderr, "Error, unable to bind to socket\n");
		exit(0);
	}

	listen(server_socket, 5);

	while (1) {
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, http_response, sizeof(http_response), 0);
		shutdown(client_socket, SHUT_RDWR);
	}

	return 0;
}

