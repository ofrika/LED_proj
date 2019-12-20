#include <stdio.h>//compile: gcc TCPServer_windows.c -o TCPServer_windows.exe -lws2_32
#include <winsock.h> //Warning: This program does not perform any error handling.

#define BUFF_SIZE 8192

int main(int argc, char *argv[])
{
    int server_socket, client_socket, addrSize, bytesRead = 0;
    struct sockaddr_in serverAddr, clientAddr;
    WSADATA wsaData;

    char buff_in[BUFF_SIZE];
    char buff_out[BUFF_SIZE];
    char buff_user_input[BUFF_SIZE];
    memset(buff_in, BUFF_SIZE, '\0');
    memset(buff_out, BUFF_SIZE, '\0');
    memset(buff_user_input, BUFF_SIZE, '\0');

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; //IPv4 address
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //don't care network interface
    serverAddr.sin_port = htons(7833); //bind to port 7833
    addrSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 0), &wsaData);               //Initialize WinSock
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Allocate TCP socket
    bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//(*@\serverBox{1)}@*)
    
	// while(1){
		listen(server_socket, 1);  //(*@\serverBox{2)}@*)
		printf("listening\n");

		client_socket = accept(server_socket, (struct sockaddr *) &clientAddr, &addrSize); //(*@\serverBox{3)}@*)
		printf("New connection from %s\n", inet_ntoa(clientAddr.sin_addr));

		while(1)
		{
			printf("Please enter something to send to Server:\n");
			scanf("%s", buff_user_input);
			strcpy(buff_out, buff_user_input);

			//Send some data
			if(send(client_socket , buff_out , strlen(buff_out) , 0) < 0)
			{
				printf("ERROR in sending. \n");
				return 1;
			}

			//Receive a reply from the server
			int num;
			if( (num = recv(client_socket , buff_in , BUFF_SIZE , 0)) < 0)
			{
				printf("ERROR in receiving. \n");
				break;
				
			} else if(num == 0){
				printf("Connection was closed. \n");		
				break;
				
			} else {
				for(int i = 0; i<num; i++){
					printf("%c", buff_in[i], num);
				}
				printf("\n");
			}
			printf("************************ DONE **********************\n");
		}
		closesocket(client_socket);
	
	//}
    closesocket(server_socket);

    WSACleanup();
    printf("Exit\n");

}