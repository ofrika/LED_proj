#include <stdio.h>//compile: gcc TCPClient_windows.c -o TCPClient_windows.exe -lws2_32
#include <winsock.h> //Warning: This program does not perform any error handling.
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")


#define BUFF_SIZE 256


/*

void recv_hello(int sockfd) {
	char recv_buff[N] = "";
	int i = 0;

	while ((recv(sockfd, &(recv_buff[i]), 1, 0) == 1)) {
		i++;
	}
	recv_buff[i] = '\0';
	printf("%s\n", recv_buff);
}

int send_size(int client_id) {
	//    char data[N] = "";        // the final version is: "size : heigt,width"
	char* data = malloc(sizeof(char)*N);
	int width, height;
	int flag = 1;
	while (flag) {
		printf("please enter the number of matrices vertically:\n");
		scanf_s("%d", &height);
		printf("please enter the number of matrices horizontally:\n");
		scanf_s("%d", &width);
		flag = 0;
		if (height*width > N) {
			printf("sorry, you should choose a smaller board\n");
			flag = 1;
		}
		free(data);
	}

	snprintf(data, N, "size:%d,%d", height, width); // puts string into buffer
	printf("%s\n", data);
	if (send(client_id, data, N, 0) == -1) {
		printf("error !! \n");
	}
	return height * width;
}


void send_orientations(int client_id, int num_of_mats) {
	//    char arr_of_orientations[num_of_mats+1];
	char data[N] = "orientations: ";
	int n = strlen(data);

	printf("for each matrix (line by line; left to right), please enter its orientations (separated by nothing!):\n");
	for (int i = 0; i <= num_of_mats; i++) {
		scanf_s("%c", &(data[i + n - 1]));     
	}
	send(client_id, data, N, 0);  //(*@\clientBox{3)}@*) send 1 byte of data to client, flags=0

}

*/

int main(int argc, char *argv[]) {

	int server_socket, bytesRead = 0;
	struct sockaddr_in address;
	WSADATA wsaData;
	char buff_in[BUFF_SIZE];
	char buff_out[BUFF_SIZE];
	memset(buff_in, BUFF_SIZE, '\0');
	memset(buff_out, BUFF_SIZE, '\0');

	WSAStartup(MAKEWORD(2, 0), &wsaData); //Initialize WinSock
	server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Allocate TCP Socket
	memset(&address, 0, sizeof(address)); //clear socket address
	address.sin_family = AF_INET;  //IPv4 address
	address.sin_addr.s_addr = inet_addr("132.69.204.167");//set to (loopback) IP address
	address.sin_port = htons(9999); //make port in network byte order
	connect(server_socket, (struct sockaddr *)&address, sizeof(address)); //(*@\clientBox{1+2)}@*)

	strcpy_s(buff_out, BUFF_SIZE, "Hello, Server!\n");
	if (send(server_socket, buff_out, BUFF_SIZE, 0) == -1)
		printf("OOPS! Error in sending data from client to server\n");
	while (1)
	{
		bytesRead = recv(server_socket, buff_in, BUFF_SIZE, 0);
		if (bytesRead != 0)
		{
			printf_s("I just got: %s from server\n", buff_in);
			printf_s("I am doing something... \n");
			strcpy_s(buff_out, BUFF_SIZE, "Successful action!\n");
			if (send(server_socket, buff_out, BUFF_SIZE, 0) == -1)
				printf("OOPS! Error in sending data from client to server\n");
		}
	}

	closesocket(server_socket);
	WSACleanup();
	return 0;
}