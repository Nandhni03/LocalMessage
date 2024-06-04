# LocalMessage
Operating Systems 2 Project 2024

# Simple Server and Chat Program

## Overview
This project implements a simple chat program with a server that can handle multiple clients. Each client connects to the server and can send messages that are broadcast to all connected clients. The server and client programs are written in C++ and use POSIX threads for handling multiple clients.

## Features
- Multi-client support using threads.
- Simple text-based communication.
- Usernames for identifying clients in the chat.

## Requirements
- C++11 or later
- CMake 3.10 or later
- POSIX-compliant system (e.g., Linux, macOS)

## Setup and Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Nandhni03/LocalMessage.git
cd simple-version
```

### 2. Build the Project
Create a build directory and run CMake to generate the build files, then compile the project.

```bash
mkdir build
cd build
cmake ..
make
```

### 3. Run the Server
Start the server by running the `server` executable.

```bash
./server
```

### 4. Run the Clients
In separate terminal windows, navigate to the `build` directory and start the client executables. Each client will prompt for a username upon starting.

```bash
./client
./client
```

### Usage
- After starting the server, clients can connect by running the client executable.
- Each client will be prompted to enter a username.
- Clients can send messages which will be broadcast to all connected clients.
- Type `#` to end the connection and exit.

## Code Structure
- `server.cpp`: Contains the implementation of the server that can handle multiple clients using threads.
- `client.cpp`: Contains the implementation of the client that connects to the server and sends/receives messages.
- `CMakeLists.txt`: CMake configuration file for building the project.

## Example Interaction
```plaintext
Server:
=> Socket server has been created...
=> Looking for clients...

Client 1:
=> Socket client has been created...
Enter your username: Nandhni
=> Connection to the server 127.0.0.1 with port number: 1501
=> Awaiting confirmation from the server...
=> Connection confirmed, you are good to go...

Client 2:
=> Socket client has been created...
Enter your username: Hemansi
=> Connection to the server 127.0.0.1 with port number: 1501
=> Awaiting confirmation from the server...
=> Connection confirmed, you are good to go...

Nandhni: Hi Hemansi!
Hemansi: Hello Nandhni!
```

## Troubleshooting
If you encounter any issues, ensure that:
- The server is running before starting the clients.
- The port number in `client.cpp` matches the port number in `server.cpp`.
- Network configuration allows communication on the specified port.
- just good luck... :)

## happy coding
