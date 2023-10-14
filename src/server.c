#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

int clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Função que lida com as conexões dos clientes
void *handle_client(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[1024];
    int read_size;

    while ((read_size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        
        // Bloquear o acesso à lista de clientes para evitar concorrência
        pthread_mutex_lock(&mutex);

        // Enviar a mensagem para todos os clientes, exceto o remetente
        for (int i = 0; i < client_count; i++) {
            if (clients[i] != client_socket) {
                send(clients[i], buffer, strlen(buffer), 0);
            }
        }

        pthread_mutex_unlock(&mutex);
        memset(buffer, 0, sizeof(buffer));
    }

    if (read_size == 0) {
        // Cliente desconectado
        printf("Cliente desconectado\n");
        
        // Remover o cliente da lista de clientes
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            if (clients[i] == client_socket) {
                for (int j = i; j < client_count - 1; j++) {
                    clients[j] = clients[j + 1];
                }
                client_count--;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    } else if (read_size == -1) {
        perror("recv falhou");
    }

    free(socket_desc);
    close(client_socket);
    return NULL;
}

// Função para enviar mensagens para todos os clientes periodicamente
void *send_messages(void *unused) {
    while (1) {
        char message[] = "Mensagem do servidor para todos os clientes.";
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            send(clients[i], message, strlen(message), 0);
        }
        pthread_mutex_unlock(&mutex);
        sleep(10); // Envie a cada 10 segundos, ajuste conforme necessário
    }
}

int main() {
    int server_socket, new_socket, c;
    struct sockaddr_in server, client;
    pthread_t thread, message_thread;

    // Inicializar a lista de clientes
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i] = 0;
    }

    // Criação do socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erro na criação do socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Vincular o socket à porta
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro no bind");
        return 1;
    }

    // Iniciar o servidor
    listen(server_socket, MAX_CLIENTS);

    printf("Aguardando conexões...\n");

    c = sizeof(struct sockaddr_in);

    // Aceitar conexões e criar threads para lidar com os clientes
    while ((new_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&c)) > 0) {
        printf("Conexão aceita\n");

        // Adicionar o novo cliente à lista de clientes
        pthread_mutex_lock(&mutex);
        clients[client_count++] = new_socket;
        pthread_mutex_unlock(&mutex);

        int *client_sock = malloc(sizeof(int));
        *client_sock = new_socket;

        if (pthread_create(&thread, NULL, handle_client, (void *)client_sock) < 0) {
            perror("Erro na criação da thread");
            return 1;
        }

        // Libere a thread quando ela terminar
        pthread_detach(thread);
    }

    if (new_socket < 0) {
        perror("Erro na aceitação da conexão");
        return 1;
    }

    // Iniciar a thread para enviar mensagens periodicamente
    if (pthread_create(&message_thread, NULL, send_messages, NULL) < 0) {
        perror("Erro na criação da thread de envio de mensagens");
        return 1;
    }

    // Aguardar o encerramento da thread de envio de mensagens
    pthread_join(message_thread, NULL);

    return 0;
}