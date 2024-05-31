#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048

volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[NAME_LEN];

void str_overwrite_stdout()
{
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf(char *arr, int lenght)
{
    for (int i = 0; i < lenght; i++) //trim \n
    { 
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

void catch_ctrl_c_and_exit()
{
    flag = 1;
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);

    signal(SIGINT, catch_ctrl_c_and_exit);

    printf("Enter your name: ");
    fgets(name, NAME_LEN, stdin);
    str_trim_lf(name, strlen(name));

    if(strlen(name) > NAME_LEN - 1 || strlen(name) < 2) {
        printf("Enter name correctly\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    // Socket Settings
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    // Connect the client to the server
    int err = connect(sockfd, (struct sockaddr* )&server_addr, sizeof(server_addr));
    if (err == -1) {
        printf("ERROR: connection failed\n");
        return EXIT_FAILURE;
    }

    //Send the name
    send(sockfd, name, NAME_LEN, 0);
    
    printf("===wElCoMe tO lOcAlChAt===\n");



    return EXIT_SUCCESS;
}












// #include <stdio.h>
// #include <sys/socket.h>

// int main() {

//     int socketFD = socket(AF_INET, SOCK_STREAM, 0);

//     char* ip = "142.250.188.46";
//     struct sockaddr_in address;
//     address.sin_port = AF_INET;
//     address.sin_port = htons(80);
//     inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

//     int result = connect(socketFD, &address, sizeof address);

//     if(result == 0)
//         printf("connection was succesful\n");


//     return 0;
// }





// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// int main(int argc, char **argv) {
//     if (argc != 3) {
//         printf("Usage: %s <IP> <Port>\n", argv[0]);
//         return EXIT_FAILURE;
//     }

//     char *ip = argv[1];
//     int port = atoi(argv[2]);
//     int sockfd;
//     struct sockaddr_in serv_addr;

//     // Create socket
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) {
//         perror("Socket creation failed");
//         return EXIT_FAILURE;
//     }

//     // Set server address
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(port);
//     if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
//         perror("Invalid address or address not supported");
//         return EXIT_FAILURE;
//     }

//     // Connect to server
//     if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Connection failed");
//         return EXIT_FAILURE;
//     }

//     printf("Connected to the server.\n");
//     char buffer[1024];
//     while (1) {
//         printf("Enter message: ");
//         fgets(buffer, 1024, stdin);
//         send(sockfd, buffer, strlen(buffer), 0);
//         if (strncmp(buffer, "exit", 4) == 0) {
//             printf("Disconnected from server.\n");
//             break;
//         }
//     }

//     close(sockfd);
//     return 0;
// }
