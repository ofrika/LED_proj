#include <stdio.h>//compile: gcc TCPServer_windows.c -o TCPServer_windows.exe -lws2_32
#include <winsock.h> //Warning: This program does not perform any error handling.

#define BUFF_SIZE 256

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
    serverAddr.sin_port = htons(5001); //bind to port 9999
    addrSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 0), &wsaData);               //Initialize WinSock
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Allocate TCP socket
    bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//(*@\serverBox{1)}@*)
    listen(server_socket, 1);  //(*@\serverBox{2)}@*)

//    for (int j = 5; (--j) >= 0;) {
    client_socket = accept(server_socket, (struct sockaddr *) &clientAddr, &addrSize); //(*@\serverBox{3)}@*)
    printf("New connection from %s\n", inet_ntoa(clientAddr.sin_addr));
    bytesRead = recv(client_socket, buff_in, BUFF_SIZE, 0);
    printf("I received From the Server %s\n", buff_in);
    printf("Please enter something to send to Server:\n");
    scanf("%s", buff_user_input);
    strcpy(buff_out, buff_user_input);
    send(client_socket, buff_out, BUFF_SIZE, 0);
//        if (bytesRead != 0)
//        {
//            if (strncmp(buff_in, "Hello, Server!\n", 14) == 0)
//            {
//                printf("I just got from Server: %s\n", buff_in);
//                while (1)
//                {
//                    printf("Please enter something to send to Server:\n");
//                    scanf_s("%s", buff_user_input, BUFF_SIZE);
//                    strcpy_s(buff_out, BUFF_SIZE, buff_user_input);
//                    if (send(client_socket, buff_out, BUFF_SIZE, 0) == -1)
//                        printf("OOPS! Error in sending data from server to Server\n");
//                    else
//                    {
//                        bytesRead = recv(client_socket, buff_in, BUFF_SIZE, 0);
//                        if (bytesRead != 0)
//                        {
//                            printf("Received from Server: %s\n", buff_in);
//                        }
//                    }
//                }
//            }
//        }

    closesocket(client_socket); //(*@\clientBox{4)}@*)
//    }

    closesocket(server_socket);  //(*@\serverBox{5)}@*)
    WSACleanup(); //Finalize WinSock
    printf("Exit\n");

}