#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define PORT 9990

int main() {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in srv{};
    srv.sin_port = htons(PORT);
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&srv, sizeof(srv));
    listen(server_fd, 10);

    fd_set master, copy;
    FD_ZERO(&master);
    FD_SET(server_fd, &master);

    int maxfd = server_fd;

    cout << "Server started...\n";

    while (1) {

        copy = master;

        int ret = select(maxfd + 1, &copy, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            break;
        }

        for (int i = 0; i <= maxfd; i++) {

            if (FD_ISSET(i, &copy)) {

                // 🔥 NEW CLIENT
                if (i == server_fd) {

                    int client = accept(server_fd, NULL, NULL);
                    cout << "New Client: " << client << endl;

                    FD_SET(client, &master);
                    if (client > maxfd) maxfd = client;

                    string msg = "Client " + to_string(client) + " joined\n";

                    for (int j = 0; j <= maxfd; j++) {
                        if (FD_ISSET(j, &master) && j != server_fd && j != client) {
                            send(j, msg.c_str(), msg.size(), 0);
                        }
                    }
                }

                // 🔥 CLIENT DATA
                else {

                    char buffer[1024] = {0};
                    int n = recv(i, buffer, sizeof(buffer), 0);

                    // 🔴 DISCONNECT
                    if (n <= 0) {
                        cout << "Client disconnected: " << i << endl;

                        close(i);
                        FD_CLR(i, &master);

                        string msg = "Client " + to_string(i) + " left\n";

                        for (int j = 0; j <= maxfd; j++) {
                            if (FD_ISSET(j, &master) && j != server_fd) {
                                send(j, msg.c_str(), msg.size(), 0);
                            }
                        }
                    }

                    // 🟢 MESSAGE
                    else {
                        cout << "Msg from " << i << ": " << buffer << endl;

                        string msg = "Client " + to_string(i) + ": " + buffer;

                        for (int j = 0; j <= maxfd; j++) {
                            if (FD_ISSET(j, &master) && j != server_fd && j != i) {
                                send(j, msg.c_str(), msg.size(), 0);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}