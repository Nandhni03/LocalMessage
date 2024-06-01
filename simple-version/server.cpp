#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm> // Include this for std::remove

using namespace std;

vector<int> clients;
mutex clients_mutex;

void handle_client(int client_socket, string client_name) {
    char buffer[1024];
    int bufsize = 1024;
    bool isExit = false;

    // Send confirmation message to the client
    string confirmation_message = "=> Connection confirmed, you are good to go...\n";
    send(client_socket, confirmation_message.c_str(), confirmation_message.length(), 0);

    while (!isExit) {
        memset(buffer, 0, bufsize);
        int bytesReceived = recv(client_socket, buffer, bufsize, 0);
        if (bytesReceived <= 0) {
            cout << client_name << " has disconnected." << endl;
            isExit = true;
        } else {
            cout << client_name << ": " << buffer << endl;

            // Broadcast the message to all other clients
            lock_guard<mutex> guard(clients_mutex);
            for (int client : clients) {
                if (client != client_socket) {
                    send(client, buffer, bytesReceived, 0);
                }
            }
        }
    }

    close(client_socket);
    lock_guard<mutex> guard(clients_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

int main() {
    int server_socket;
    int portNum = 1500;
    struct sockaddr_in server_addr;
    socklen_t size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "\n=> Socket server has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    cout << "=> Looking for clients..." << endl;
    listen(server_socket, 5);

    while (true) {
        int client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &size);
        if (client_socket < 0) {
            cout << "=> Error on accepting..." << endl;
            continue;
        }

        char buffer[1024];
        memset(buffer, 0, 1024);
        recv(client_socket, buffer, 1024, 0);
        string client_name(buffer);

        cout << client_name << " has connected." << endl;

        {
            lock_guard<mutex> guard(clients_mutex);
            clients.push_back(client_socket);
        }

        thread(handle_client, client_socket, client_name).detach();
    }

    close(server_socket);
    return 0;
}





// #include <iostream>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <thread>
// #include <vector>
// #include <mutex>

// using namespace std;

// vector<thread> clientThreads;
// mutex coutMutex;

// void handleClient(int server)
// {
//     int bufsize = 1024;
//     char buffer[bufsize];
//     bool isExit = false;

//     // Receive username
//     recv(server, buffer, bufsize, 0);
//     string username(buffer);
//     coutMutex.lock();
//     cout << "=> " << username << " connected..." << endl;
//     coutMutex.unlock();

//     do {
//         coutMutex.lock();
//         cout << username << ": ";
//         coutMutex.unlock();
//         do {
//             recv(server, buffer, bufsize, 0);
//             coutMutex.lock();
//             cout << buffer << " ";
//             coutMutex.unlock();
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);

//         if (isExit)
//             break;

//         coutMutex.lock();
//         cout << "\nServer: ";
//         coutMutex.unlock();
//         do {
//             cin.getline(buffer, bufsize);
//             send(server, buffer, bufsize, 0);
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);

//     } while (!isExit);

//     coutMutex.lock();
//     cout << "\n=> Connection terminated with " << username << "." << endl;
//     coutMutex.unlock();
//     close(server);
// }

// int main()
// {
//     int client;
//     int portNum = 1500;
//     int server;
//     struct sockaddr_in server_addr;
//     socklen_t size;

//     client = socket(AF_INET, SOCK_STREAM, 0);
//     if (client < 0) 
//     {
//         cout << "\nError establishing socket..." << endl;
//         return -1;
//     }

//     // Set SO_REUSEADDR to allow reuse of the address
//     int opt = 1;
//     if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
//         cout << "\nError setting socket options..." << endl;
//         return -1;
//     }

//     cout << "\n=> Socket server has been created..." << endl;

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(portNum);

//     if ((bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0) 
//     {
//         cout << "=> Error binding connection, the socket has already been established..." << endl;
//         return -1;
//     }

//     size = sizeof(server_addr);
//     cout << "=> Looking for clients..." << endl;

//     listen(client, 5);

//     while (true)
//     {
//         server = accept(client, (struct sockaddr *)&server_addr, &size);

//         if (server < 0) 
//         {
//             cout << "=> Error on accepting..." << endl;
//             continue;
//         }

//         clientThreads.push_back(thread(handleClient, server));
//     }

//     for (auto& th : clientThreads)
//     {
//         if (th.joinable())
//         {
//             th.join();
//         }
//     }

//     close(client);
//     return 0;
// }


// /*!
//  * Simple chat program (server side).cpp - http://github.com/hassanyf
//  * Version - 2.0.1
//  *
//  * Copyright (c) 2016 Hassan M. Yousuf
//  */

// #include <iostream>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <thread>
// #include <vector>
// #include <mutex>

// using namespace std;

// vector<thread> clientThreads; // Store client threads
// mutex coutMutex; // Protects cout access

// void handleClient(int server)
// {
//     int bufsize = 1024;
//     char buffer[bufsize];
//     bool isExit = false;

//     strcpy(buffer, "=> Server connected...\n");
//     send(server, buffer, bufsize, 0);
//     cout << "=> Client connected, you are good to go..." << endl;

//     cout << "\n=> Enter # to end the connection\n" << endl;

//     do {
//         coutMutex.lock();
//         cout << "Client: ";
//         coutMutex.unlock();
//         do {
//             recv(server, buffer, bufsize, 0);
//             coutMutex.lock();
//             cout << buffer << " ";
//             coutMutex.unlock();
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);

//         if (isExit)
//             break;

//         coutMutex.lock();
//         cout << "\nServer: ";
//         coutMutex.unlock();
//         do {
//             cin.getline(buffer, bufsize);
//             send(server, buffer, bufsize, 0);
//             if (strcmp(buffer, "#") == 0) {
//                 isExit = true;
//                 break;
//             }
//         } while (strcmp(buffer, "#") != 0);

//     } while (!isExit);

//     cout << "\n=> Connection terminated with client." << endl;
//     close(server);
// }

// int main()
// {
//     int client;
//     int portNum = 1500;
//     int server;
//     struct sockaddr_in server_addr;
//     socklen_t size;

//     client = socket(AF_INET, SOCK_STREAM, 0);
//     if (client < 0)
//     {
//         cout << "\nError establishing socket..." << endl;
//         return -1;
//     }

//     cout << "\n=> Socket server has been created..." << endl;

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = htons(INADDR_ANY);
//     server_addr.sin_port = htons(portNum);

//     if ((bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
//     {
//         cout << "=> Error binding connection, the socket has already been established..." << endl;
//         return -1;
//     }

//     size = sizeof(server_addr);
//     cout << "=> Looking for clients..." << endl;

//     listen(client, 5);

//     while (true)
//     {
//         server = accept(client, (struct sockaddr *)&server_addr, &size);

//         if (server < 0)
//         {
//             cout << "=> Error on accepting..." << endl;
//             continue;
//         }

//         clientThreads.push_back(thread(handleClient, server));
//     }

//     for (auto& th : clientThreads)
//     {
//         if (th.joinable())
//         {
//             th.join();
//         }
//     }

//     close(client);
//     return 0;
// }
