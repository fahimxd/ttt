#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <fcntl.h>
#include <vector>
#include <thread>

using namespace std;


int main() {

    // create server-socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listening, F_SETFL, O_NONBLOCK);

    if (listening == -1) {
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    // bind socket to ip/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // check if can bind
    //if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
    //    cerr << "Failed to bind to IP/port" << endl;
    //    return -2;
    //}

    // check if can listen
    if (listen(listening, SOMAXCONN) == -1) {
        cerr << "Failed to listen." << endl;
        return -3;
    }

    
    //


    fd_set master;
    FD_ZERO(&master);
    FD_SET(listening, &master);

    while (true) {

        for (int i=0; i<=FD_SETSIZE; i++) {
            if (FD_ISSET(i, &master)) {
                int sock = i;

                if(sock == listening) {
                    sockaddr_in client;
                    socklen_t clientSize = sizeof(client);
                    char host[NI_MAXHOST];
                    char svc[NI_MAXSERV];

                    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
                    FD_SET(clientSocket, &master);

                    string acceptMessage = "Successfully connected to the server.";
                    send(clientSocket, acceptMessage.c_str(), (acceptMessage.size() + 1), 0);
                }
                else {
                    char buffer[4096];
                    memset(buffer, 0, 4096);

                    // receive from client
                    int bytesReceived = recv(sock, buffer, 4096, MSG_DONTWAIT);

                    if (bytesReceived == -1) {
                        ;
                    }
                    else if (bytesReceived == 0) {
                        close(sock);
                        FD_CLR(sock, &master);
                        cout << "Client disconnected." << endl;
                    }
                    else {
                        cout << "received: " << string(buffer, 0, bytesReceived) << endl;

                        for (int j=0; j<100; j++) {
                            int outSock = j;
                            if (FD_ISSET(j, &master)) {
                                if (outSock != listening && outSock != 63) {
                                    if (outSock == sock) {
                                        string echoMsg = "<echo>" + string(buffer, 0, bytesReceived);
                                        send(outSock, echoMsg.c_str(), (echoMsg.size() + 1), 0);
                                        ;
                                    }
                                    else {
                                        send(outSock, buffer, bytesReceived, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
