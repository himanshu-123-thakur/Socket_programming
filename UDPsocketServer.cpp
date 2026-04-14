//In UDP, since there is no connection, the server tracks clients 
// using IP and port and implements broadcast by iterating over 
// stored client addresses.

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 9909

bool isSameClient(sockaddr_in a, sockaddr_in b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr &&
           a.sin_port == b.sin_port;
}

int main() {

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in srv{}, client{};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (sockaddr*)&srv, sizeof(srv));

    vector<sockaddr_in> clients;

    cout << "UDP Server started...\n";

    while (1) {

        char buffer[1024] = {0};
        socklen_t len = sizeof(client);

        int n = recvfrom(sock, buffer, sizeof(buffer), 0,
                         (sockaddr*)&client, &len);

        if (n <= 0) continue;

        buffer[n] = '\0';

        cout << "[RECV] "
             << inet_ntoa(client.sin_addr)
             << ":" << ntohs(client.sin_port)
             << " → " << buffer << endl;

        // add new client
        bool found = false;
        for (auto &c : clients) {
            if (isSameClient(c, client)) {
                found = true;
                break;
            }
        }

        if (!found) {
            clients.push_back(client);
        }

        // broadcast
        for (auto &c : clients) {
            if (!isSameClient(c, client)) {
                sendto(sock, buffer, strlen(buffer), 0,
                       (sockaddr*)&c, sizeof(c));
            }
        }
    }
}