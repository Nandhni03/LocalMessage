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

using namespace std; // mot good practice...

vector<int> clients;
mutex clients_mutex; //ensures thread safe access to the clients vector


void handle_client(int client_socket, string client_name)
{
    char buffer[1024];
    int bufsize = 1024;
    bool isExit = false;

    // Send confirmation message to the client
    string confirmation_message = "=> Connection confirmed, you are good to go...\n";
    send(client_socket, confirmation_message.c_str(), confirmation_message.length(), 0);
    
    // receiving message 
    while (!isExit)
    {
        memset(buffer, 0, bufsize);
        int bytesReceived = recv(client_socket, buffer, bufsize, 0);
        if (bytesReceived <= 0)
        {
            cout << client_name << " has disconnected." << endl;
            isExit = true;
        }
        else
        {
            cout << client_name << ": " << buffer << endl;

            // Broadcast the message to all other clients
            if (bytesReceived > 0)
            {
                string message = client_name + ": " + string(buffer, 0, bytesReceived);
                lock_guard<mutex> guard(clients_mutex);
                for (int client : clients)
                {
                    if (client != client_socket)
                    {
                        send(client, message.c_str(), message.length(), 0);
                    }
                }
            }
        }
    }

    close(client_socket);
    lock_guard<mutex> guard(clients_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

int main()
{
    int server_socket;
    int portNum = 1500;
    struct sockaddr_in server_addr;
    socklen_t size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "\n=> Socket server has been created..." << endl;
    
    // server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    cout << "=> Looking for clients..." << endl;
    listen(server_socket, 5);
    
    //accept and handle clients
    while (true)
    {
        int client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &size);
        if (client_socket < 0)
        {
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
