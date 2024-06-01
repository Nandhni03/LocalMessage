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
        cout << "Server: " << buffer << endl;
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

    do {
        cout << client_name << ": ";
        cin >> buffer;
        send(client, buffer, bufsize, 0);
        if (*buffer == '#') {
            isExit = true;
        }
    } while (!isExit);

    close(client);
    return 0;
}






// #include <iostream>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// using namespace std;

// int main()
// {
//     int client;
//     int portNum = 1500;
//     bool isExit = false;
//     int bufsize = 1024;
//     char buffer[bufsize];
//     char* ip = "127.0.0.1";

//     struct sockaddr_in server_addr;

//     client = socket(AF_INET, SOCK_STREAM, 0);

//     if (client < 0) 
//     {
//         cout << "\nError establishing socket..." << endl;
//         return -1;
//     }

//     cout << "\n=> Socket client has been created..." << endl;

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(portNum);

//     inet_pton(AF_INET, ip, &server_addr.sin_addr);

//     if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
//         cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;

//     cout << "=> Awaiting confirmation from the server..." << endl;
//     recv(client, buffer, bufsize, 0);
//     cout << "=> Connection confirmed, you are good to go..." << endl;

//     cout << "\n\n=> Enter # to end the connection\n" << endl;

//     cout << "Enter your username: ";
//     string username;
//     cin >> username;
//     send(client, username.c_str(), bufsize, 0);

//     do {
//         cout << "Client: ";
//         do {
//             cin >> buffer;
//             send(client, buffer, bufsize, 0);
//             if (strcmp(buffer, "#") == 0) {
//                 send(client, buffer, bufsize, 0);
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);

//         if (isExit)
//             break;

//         cout << "Server: ";
//         do {
//             recv(client, buffer, bufsize, 0);
//             cout << buffer << " ";
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);
//         cout << endl;

//     } while (!isExit);

//     cout << "\n=> Connection terminated.\nGoodbye...\n";
//     close(client);
//     return 0;
// }


// /*!
//  * Simple chat program (client side).cpp - http://github.com/hassanyf
//  * Version - 2.0.1
//  *
//  * Copyright (c) 2016 Hassan M. Yousuf
//  */

// #include <iostream>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// using namespace std;

// int main()
// {
//     int client;
//     int portNum = 1500; // Port number
//     bool isExit = false;
//     int bufsize = 1024;
//     char buffer[bufsize];
//     char* ip = "127.0.0.1";

//     struct sockaddr_in server_addr;

//     client = socket(AF_INET, SOCK_STREAM, 0);
//     if (client < 0)
//     {
//         cout << "\nError establishing socket..." << endl;
//         return -1;
//     }

//     cout << "\n=> Socket client has been created..." << endl;

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(portNum);
//     inet_pton(AF_INET, ip, &server_addr.sin_addr);

//     if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
//         cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;

//     cout << "=> Awaiting confirmation from the server..." << endl;
//     recv(client, buffer, bufsize, 0);
//     cout << "=> Connection confirmed, you are good to go..." << endl;

//     cout << "\n\n=> Enter # to end the connection\n" << endl;

//     do {
//         cout << "Client: ";
//         do {
//             cin.getline(buffer, bufsize);
//             send(client, buffer, bufsize, 0);
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//             }
//         } while (strcmp(buffer, "#") != 0);

//         cout << "Server: ";
//         do {
//             recv(client, buffer, bufsize, 0);
//             cout << buffer << " ";
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//             }
//         } while (strcmp(buffer, "#") != 0);
//         cout << endl;

//     } while (!isExit);

//     cout << "\n=> Connection terminated.\nGoodbye...\n";
//     close(client);
//     return 0;
// }
