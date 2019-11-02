#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <io.h>

#define BUFF_SIZE 256
#define PORT 9999
#define SA struct sockaddr

// Function designed for chat between client and server.
void chat(int sockfd)
{
    char buff[BUFF_SIZE];
    int n;
    // infinite loop for chat
    for (;;) {
        for(int i=0; i<BUFF_SIZE; i++){
            buff[i] = 0;
        }
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\n", buff);

        for(int i=0; i<BUFF_SIZE; i++){
            buff[i] = 0;
        }
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
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
    serverAddr.sin_port = htons(PORT); //bind to port 9999
    addrSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 0), &wsaData);               //Initialize WinSock
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Allocate TCP socket
    bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//(*@\serverBox{1)}@*)
    listen(server_socket, 5);  //(*@\serverBox{2)}@*)


    // Accept the data packet from client and verification
    int connfd = accept(server_socket, (SA*)&clientAddr, &addrSize);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    chat(connfd);

    closesocket(server_socket);  //(*@\serverBox{5)}@*)
    WSACleanup(); //Finalize WinSock
}
