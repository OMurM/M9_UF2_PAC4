#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 9100 // Puerto del servidor
#define SERVER_IP "127.0.0.1" // Dirección IP del servidor (localhost)

// Función para conectar al servidor y enviar una comanda
void enviarComanda(const string& comanda) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir la IP del servidor
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Enviar la comanda al servidor
    send(sock, comanda.c_str(), comanda.length(), 0);
    cout << "Comanda enviada: " << comanda << endl;

    // Recibir el identificador de la comanda
    char buffer[1024] = {0};
    int valread = read(sock, buffer, sizeof(buffer));
    if (valread > 0) {
        cout << "Identificador de la comanda recibido: " << buffer << endl;
    }

    // Cerrar el socket
    close(sock);
}

int main() {
    string comanda;

    // Solicitar la comanda al usuario
    cout << "Introduïu la vostra comanda: ";
    getline(cin, comanda);

    // Enviar la comanda al servidor
    enviarComanda(comanda);

    return 0;
}
