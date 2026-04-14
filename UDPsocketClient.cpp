//In UDP, since there is no connection, the server tracks clients 
// using IP and port and implements broadcast by iterating over 
// stored client addresses.

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 9909

int main() {

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in srv{};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1"); // FIXED

    fd_set master, copy;

    cout << "UDP Client started...\n";

    while (1) {

        FD_ZERO(&master);
        FD_SET(0, &master);     // stdin
        FD_SET(sock, &master);  // socket

        copy = master;   // 🔥 VERY IMPORTANT

        select(sock + 1, &copy, NULL, NULL, NULL);

        // 🔹 USER INPUT → SEND
        if (FD_ISSET(0, &copy)) {
            char buffer[1024] = {0};
            cin.getline(buffer, sizeof(buffer));

            if (strcmp(buffer, "exit") == 0) {
                cout << "Exiting...\n";
                break;
            }

            sendto(sock, buffer, strlen(buffer), 0,
                   (sockaddr*)&srv, sizeof(srv));
        }

        // 🔹 RECEIVE FROM SERVER
        if (FD_ISSET(sock, &copy)) {
            char buffer[1024] = {0};
            socklen_t len = sizeof(srv);

            int n = recvfrom(sock, buffer, sizeof(buffer), 0,
                             (sockaddr*)&srv, &len);

            if (n > 0) {
                buffer[n] = '\0';
                cout << "\n[MSG] " << buffer << endl;
            }
        }
    }

    close(sock);
}