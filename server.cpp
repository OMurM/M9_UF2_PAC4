#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 9100

mutex fileMutex;

string generarIdentificador(int id) {
    return "ORD-" + to_string(id); // Formato: ORD-0001, ORD-0002, etc.
}

void manejarCliente(int clientSocket, int id) {
    char buffer[1024];
    bzero(buffer, 1024);

    read(clientSocket, buffer, sizeof(buffer));
    string comanda = buffer;

    string identificador = generarIdentificador(id);

    {
        lock_guard<std::mutex> lock(fileMutex);
        ofstream outFile("comandes.txt", ios::app);
        if (outFile.is_open()) {
            outFile << identificador << ": \"" << comanda << "\"" << endl;
            outFile.close();
        } else {
            cerr << "Error al abrir el archivo para escribir" << endl;
        }
    }

    send(clientSocket, identificador.c_str(), identificador.length(), 0);
    cout << "Comanda recibida: " << comanda << " - Identificador enviado: " << identificador << endl;

    close(clientSocket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    cout << ">> Servei iniciat. Escoltant al port " << PORT << "..." << endl;

    int id = 1;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        cout << ">> Connexió acceptada des del client " << inet_ntoa(address.sin_addr) << endl;

        thread clienteThread(manejarCliente, new_socket, id);
        clienteThread.detach();

        id++;
    }

    close(server_fd);
    return 0;
}

