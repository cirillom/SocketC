// C program to demonstrate peer to peer chat using Socket Programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

char name[20];
int PORT;
int PORT_server;

void sending();
void changePort();
void receiving(int server_fd);
void *receive_thread(void *server_fd);

int main()
{
    printf("Enter name: ");
    scanf("%s", name);

    printf("Enter your port number: ");
    scanf("%d", &PORT);

    int server_fd;
    struct sockaddr_in address;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //Printed the server socket addr and port
    printf("IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("port is: %d\n", (int)ntohs(address.sin_port));
    printf("To change port number enter @#1$ at any moment\n");
    printf("To exit enter @#2$ at any moment\n");

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, &server_fd); //Creating thread to keep receiving message in real time

    changePort();

    sending();

    close(server_fd);

    return 0;
}

void changePort()
{
    printf("Enter the port to send message: ");
    scanf("%d", &PORT_server);
}

//Sending messages to port
void sending()
{
    char buffer[2000] = {0};
    //Fetching port number

    char message[1024] = {0};
    char dummy;
    //printf("Enter your message:");
    scanf("%c", &dummy); //The buffer is our enemy
    scanf("%[^\n]s", message);

    //if message is @#1$ then change port
    if (strcmp(message, "@#1$") == 0)
    {
        fputs("\033[A\033[2K",stdout);
        rewind(stdout);
        changePort();
        sending();
    } //if message is @#2$ then exit
    else if (strcmp(message, "@#2$") == 0)
    {
        exit(0);
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    while ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        changePort();
    }

    sprintf(buffer, "%s[PORT:%d] says: %s", name, PORT, message);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY always gives an IP of 0.0.0.0
    serv_addr.sin_port = htons(PORT_server);

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        changePort();
    }

    send(sock, buffer, sizeof(buffer), 0);
    //unicode checkmark in the end of the previous line
    
    fputs("\033[A\033[2K",stdout);
    rewind(stdout);

    printf("%s -> [PORT:%d] says: %s \u2713 \n", name, PORT_server, message);
    close(sock);
    sending();
}

//Calling receiving every 2 seconds
void *receive_thread(void *server_fd)
{
    int s_fd = *((int *)server_fd);
    while (1)
    {
        sleep(2);
        receiving(s_fd);
    }
}

//Receiving messages on our port
void receiving(int server_fd)
{
    struct sockaddr_in address;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1)
    {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {

                if (i == server_fd)
                {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    recv(i, buffer, sizeof(buffer), 0);
                    printf("\n%s\n", buffer);
                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}