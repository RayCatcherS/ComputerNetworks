#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#define BUFFER_SIZE 512
#define SERVER_WELCOME_PORT 30000
#define SERVER_REPLY_SERVICE_PORT 30001
#define SERVER_IP "127.0.0.1"
#define QUEUE_LEN 4



#include <stdio.h>
#include <stdlib.h>

// methods
void serverSocket();
void print(char* message);

#define NO_ERROR 0
void clearwinsock() {

#if defined WIN32
	WSACleanup();
#endif
}

int main(void) {

#if defined WIN32
	// initialize Win sock
	print("WSAStartup\n");
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(result != NO_ERROR) {
		print("Error at WSAStartup()\n");
		return 0;
	}
#endif


	serverSocket();

	print("|--------------------------------|\nEnd of server execution\n");
	return EXIT_SUCCESS;
}



void serverSocket() {
	printf("Start server\n");

	// init server socket
	int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_socket < 0) {
		print("Unable to initialize the server socket\n");
		closesocket(server_socket);
		clearwinsock();
	}

	// bind socket
	struct  sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_WELCOME_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	int bind_res = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if(bind_res < 0) {
		print("Unable to bind the socket\n");
		closesocket(server_socket);
		clearwinsock();
	}

	// set socket to listen
	int listen_res = listen(server_socket, QUEUE_LEN);
	if(listen_res < 0) {
		print("Unable to listen the socket\n");
		closesocket(server_socket);
		clearwinsock();
	}

	// accept new connection
	struct sockaddr_in client_address;
	int client_socket;
	int client_address_len;


	while(1){
		print("Waiting for a new connection...\n");
		client_address_len = sizeof(client_address);
		client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_len);

		if(client_socket < 0) {
			print("Unable to accept the client\n");
			closesocket(server_socket);
			clearwinsock();
			return;
		}

		printf("client connection received from: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
		fflush(stdout);

		char received_msg[BUFFER_SIZE] = "";
		int received_code = recv(client_socket, received_msg, BUFFER_SIZE - 1, 0);

		if(received_code < 0) {
			print("Error on receive message from client\n");
			printf("error code: %d\n", received_code);
			fflush(stdout);
			return;
		}



		printf("Echoing received message: %s\n", received_msg);
		// reply to client


		send(client_socket, received_msg, strlen(received_msg), 0);
		print("Echo message sent\n");
	}
}

void print(char* message) {
	printf(message);
	fflush(stdout);
}



