#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>


int main(int argc, char *argv[])
{
    int socket_descriptor, bind_status, fd1, port, client_socket, send_size, recv_data;
    struct sockaddr_in server_address, client_address;
    char buffer[250] = {"SERVER CONNECTION SUCCESSFUL. WELCOME"};
    // checking the number of arguments
    if(argc < 2)
    {
        perror("Invalid number of arguments");
        exit(1);
    }
    // creating file for storing data sent btn client and server
    fd1 = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if(fd1 < 0)
    {
        perror("Error in creating file");
        exit(1);
    }
    // cfreating the server socket
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor < 0)
    {
        perror("Error in creating socket");
        exit(1);
    }
    // resetting the address before usage
    bzero(&server_address, sizeof(server_address));
    // assinging port and server address values
    port = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // binding the socket port and address
    bind_status = bind(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));
    if(bind_status == -1)
    {
        perror("Error binding port address to the socket");
        exit(1);
    }
    // listening for connection
    listen(socket_descriptor, 5); 
    // accepting connections
    // client_socket = sizeof(client_address);
    // bzero(&client_address, sizeof(client_socket));
    // client_socket = accept(socket_descriptor, (struct sockaddr *) &client_address, client_socket);
    client_socket = accept(socket_descriptor, NULL, NULL);
    if(client_socket < 0)
    {
        perror("Error in creating slave socket for client connection");
        exit(1);
    }

    // recieving and sending data btn server and client
    send_size = send(client_socket, buffer,sizeof(buffer), 0);
    if(send_size < 0)
    {
        perror("Error in sending data");
        exit(1);
    }
    write(fd1, buffer, sizeof(buffer));
    bzero(buffer, sizeof(buffer));
    while(true)
    {
        recv_data = recv(socket_descriptor, buffer, sizeof(buffer), 0);
        if(recv_data<0)
        {
            perror("Recieving error");
            exit(1);
        }
        printf("Client Data: \t %s \n");
        write(fd1, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        printf("Enter data to be sent to client:\t");
        scanf("%s", &buffer);
        send(socket_descriptor, buffer, sizeof(buffer), 0);
        write(fd1, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
    }
    bzero(buffer, sizeof(buffer));
    close(socket_descriptor);   
    return 0;
}