#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

using namespace std;

void receive_messages(int client_socket) {
    char buffer[1024];
    int bufsize = 1024;
    while (true) {
        memset(buffer, 0, bufsize);
        int bytesReceived = recv(client_socket, buffer, bufsize, 0);
        if (bytesReceived <= 0) {
            cout << "Connection closed by server." << endl;
            exit(1);
        }
        string message(buffer);
        cout << message << endl;
    }
}

int main() {
    int client;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    string client_name;

    cout << "Enter your name: ";
    cin >> client_name;

    struct sockaddr_in server_addr;
    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
        cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;
    } else {
        cout << "=> Connection failed." << endl;
        return -1;
    }

    send(client, client_name.c_str(), client_name.length(), 0);

    thread(receive_messages, client).detach();

    cout << "=> Enter # to end the connection\n" << endl;

    while (!isExit) {
        cout << client_name << ": ";
        cin >> buffer;
        send(client, buffer, bufsize, 0);
        if (*buffer == '#') {
            isExit = true;
        }
    }


    close(client);
    return 0;
}
