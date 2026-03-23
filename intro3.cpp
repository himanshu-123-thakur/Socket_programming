//MAIN THINS

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9900);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    while (1) {
        fd_set f;
        FD_ZERO(&f);        // RESET
        FD_SET(s, &f);      // ADD

        select(s + 1, &f, NULL, NULL, NULL);  // WAIT

        if (FD_ISSET(s, &f)) {                // CHECK
            cout << "Client trying to connect\n";
        }
    }
}