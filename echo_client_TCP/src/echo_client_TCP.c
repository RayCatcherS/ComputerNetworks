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
#define SERVER_WELCOME_PORT 30000
#define SERVER_IP "192.168.178.69"


#define NO_ERROR 0
void clearwinsock() {

#if defined WIN32
	WSACleanup();
#endif
}

// methods
void clientSocket();
void print(char* message);

int main(void) {

#if defined WIN32
	// initialize Win sock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(result != NO_ERROR) {
		print("Error at WSAStartup()\n");
		return 0;
	}
#endif

	print("Starting client\n");
	clientSocket();

	print("|--------------------------------|\nEnd of client execution\n");
	system("pause");
	return EXIT_SUCCESS;
}

//
void clientSocket() {

	int c_socket;
	// client socket
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(c_socket < 0) {
		print("Unable to initialize a new socket\n");
		closesocket(c_socket);
		clearwinsock();
		return;
	}

	// get host name for the request
	struct hostent* hostip = gethostbyname("localhost");

	if(hostip == NULL) {
		print("Error during the dns resolving");
		return;
	}

	struct in_addr* ina = (struct in_addr*) hostip->h_addr_list[0];
	char* result = inet_ntoa(*ina);




	// server addr init
	struct sockaddr_in server_soc_addr;
	server_soc_addr.sin_family = AF_INET;
	server_soc_addr.sin_port = htons(SERVER_WELCOME_PORT);
	server_soc_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// server stream connection
	int connection_result = connect(c_socket, (struct sockaddr*) &server_soc_addr, sizeof(server_soc_addr));
	if(connection_result < 0) {

		print("Unable to connect to server\n");
		closesocket(c_socket);
		clearwinsock();
		return;
	}



	// send to server the message
	char* input_string = "prova";


	int send_result = send(c_socket, input_string, strlen(input_string), 0);
	if(send_result < 0) {
		print("Unable to send data to server\n");
		closesocket(c_socket);
		clearwinsock();
		return;
	}
	print("Message to server sent\n");

	// wait echo server
	char received_msg[BUFFER_SIZE] = "";
	int received_code = recv(c_socket, received_msg, BUFFER_SIZE - 1, 0);

	if(received_code < 0) {
		print("Error on receive echo message from server\n");
		printf("error code: %d\n", received_code);
		fflush(stdout);
		return;
	}



	printf("Echoing received message: %s\n", received_msg);



	// end - close socket
	closesocket(c_socket);
	clearwinsock();
}

void print(char* message) {
	printf(message);
	fflush(stdout);
}
