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


#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512
#define PROTOPORT 27015
#define SERVER_IP "192.168.178.69"


#define NO_ERROR 0
void clearwinsock() {

#if defined WIN32
	WSACleanup();
#endif
}

// methods
void clientSocket();


int main(void) {

#if defined WIN32
	// initialize Win sock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif


	clientSocket();

	puts("\n|--------------------------------|\nEnd of client execution"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

//
void clientSocket() {
	int c_socket;
	// client socket
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(c_socket < 0) {
		printf("socket: %d\n", c_socket);
		printf("Unable to initialize a new socket\n");
		closesocket(c_socket);
		clearwinsock();
	}


	// server addr init
	struct sockaddr_in server_soc_addr;
	memset(&server_soc_addr, 0, sizeof(server_soc_addr));
	server_soc_addr.sin_family = AF_INET;
	server_soc_addr.sin_port = htons(PROTOPORT);
	server_soc_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// server stream connection
	int connection_result = connect(c_socket, (struct sockaddr*) &server_soc_addr, sizeof(server_soc_addr));
	if(connection_result < 0) {

		printf("Unable to connect to server\n");
		closesocket(c_socket);
		clearwinsock();
	}



	// send to server the message
	char* input_string = "prova";
	int input_len = strlen(input_string);


	int send_result = send(c_socket, input_string, input_len, 0);
	if(send_result < 0) {
		printf("Unable to send data to server\n");
		closesocket(c_socket);
		clearwinsock();
	}

	// end - close socket
	closesocket(c_socket);
	clearwinsock();
}
