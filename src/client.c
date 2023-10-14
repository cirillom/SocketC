#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

void *receive_messages(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char server_response[1024];

    while (1) {
        int read_size = recv(client_socket, server_response, sizeof(server_response), 0);
        if (read_size > 0) {
            server_response[read_size] = '\0';
            printf("\nMensagem do servidor: %s", server_response);
        }
    }

    return NULL;
}

int main() {
    int client_socket;
    struct sockaddr_in server;

    // Criação do socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erro na criação do socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Conectar ao servidor
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro na conexão com o servidor");
        return 1;
    }

    printf("Conectado ao servidor. Você pode enviar mensagens (ou 'exit' para sair):\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, receive_messages, (void *)&client_socket) < 0) {
        perror("Erro na criação da thread");
        return 1;
    }

    char message[1024];
    while (1) {
        // Envie uma mensagem para o servidor
        fgets(message, sizeof(message), stdin);

        if (strcmp(message, "exit\n") == 0) {
            break;
        }

        send(client_socket, message, strlen(message), 0);
    }

    // Aguarda a thread de recebimento de mensagens terminar
    pthread_join(thread, NULL);
    close(client_socket);

    return 0;
}
