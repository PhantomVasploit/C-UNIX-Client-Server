#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

int main(int argc, char *argv[])
{
    int socket_descriptor, port, connection_status, fd1;
    char buffer[250];
    struct sockaddr_in client_socket;
    if(argc < 2)
    {
        perror("Invalid number of arguments provided.");
        exit(1);
    }
    // file for storing all communication btn client and server
    fd1 = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if(fd1 < 0)
    {
        perror("Error in opening file");
        exit(1);
    }
    // creating the socket
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);// family=IPV4, type=Stream socket, protocol=TCP
    if(socket_descriptor < 0)
    {
        perror("Error creating socket");
        exit(1);
    }
    // resetting address b4 usage
    bzero(&client_socket, sizeof(client_socket));
    // port and address assingment
    port = atoi(argv[1]);
    client_socket.sin_family = AF_INET;
    client_socket.sin_port = htons(port);
    client_socket.sin_addr.s_addr = INADDR_ANY;
    //connecting to server
    connection_status = connect(socket_descriptor, (struct sockaddr *) &client_socket, sizeof(client_socket));
    if(connection_status == -1)
    {
        perror("Error connecting to server");
        exit(1);
    }
    // reciving data from socket and sending data via the socket
    while(true)
    {
        recv(socket_descriptor, buffer, sizeof(buffer), 0);
        printf("Server Data: \t %s \n", buffer);
        write(fd1, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        printf("Enter data to send to server:\t");
        scanf("%s", &buffer);
        send(socket_descriptor, buffer, sizeof(buffer), 0);
        write(fd1, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
    }
    //bzero(buffer, sizeof(buffer));
    close(socket_descriptor);
    return 0;
}