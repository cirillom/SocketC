#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server;
    char message[1024];
    char server_response[1024];

    // Criação do socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erro na criação do socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Conectar ao servidor
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro na conexão com o servidor");
        return 1;
    }

    printf("Conectado ao servidor. Digite 'exit' para sair.\n");

    while (1) {
        printf("Digite uma mensagem para enviar ao servidor: ");
        fgets(message, sizeof(message), stdin);

        // Enviar mensagem para o servidor
        send(client_socket, message, strlen(message), 0);

        if (strcmp(message, "exit\n") == 0) {
            break;
        }

        // Receber resposta do servidor
        memset(server_response, 0, sizeof(server_response));
        recv(client_socket, server_response, sizeof(server_response), 0);
        printf("Resposta do servidor: %s", server_response);
    }

    close(client_socket);

    return 0;
}
