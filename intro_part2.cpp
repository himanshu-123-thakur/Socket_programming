#include <iostream>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

struct sockaddr_in srv;
#define nPort 9900
fd_set fr, fw, fe; //A list of sockets you want to monitor

/*
fd_set Macros (used with select() for multiplexing sockets):

//64 poss

FD_ZERO(fd_set *set)
→ Clears the set (all bits = 0)
→ No file descriptors are being monitored

FD_SET(int fd, fd_set *set)
→ Adds a file descriptor to the set
→ Marks this FD as "watch for activity"

FD_CLR(int fd, fd_set *set)
→ Removes a file descriptor from the set
→ Stops monitoring this FD

FD_ISSET(int fd, fd_set *set)
→ Checks if FD is ready after select()
→ Returns true if this FD has activity (e.g., data to read)

------------------------------------------------------------

Usage Flow:

fd_set readfds;

FD_ZERO(&readfds);        // Step 1: clear set
FD_SET(sock1, &readfds);  // Step 2: add socket 1
FD_SET(sock2, &readfds);  // Step 3: add socket 2

select(max_fd + 1, &readfds, NULL, NULL, NULL);

if (FD_ISSET(sock1, &readfds)) {
    // sock1 has data
}

if (FD_ISSET(sock2, &readfds)) {
    // sock2 has data
}

------------------------------------------------------------

IMPORTANT:
select() modifies fd_set → always reinitialize or copy before reuse
*/

int main(){
    int nMaxFd;
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int nRet = 0;

    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(nPort);

    memset(&srv,0,sizeof(srv));


    //BIND
    nRet=bind(nSocket, (sockaddr*)&srv, sizeof(srv));

    //LISTEN
    nRet=listen(nSocket,5);



    nMaxFd=nSocket;

    struct timeval tv;


    //Receive connection from new client and process it

    while(1){

        tv.tv_sec =1 ;
        tv.tv_usec = 0;

    FD_ZERO(&fr);
    FD_ZERO(&fw);
    FD_ZERO(&fe);
    FD_SET(nSocket, &fr);
    FD_SET(nSocket, &fe);
    
    cout<<endl<<"Before select call : ";
    for (int i = 0; i <= nMaxFd; i++) {
        if (FD_ISSET(i, &fr)) {
            cout << i << " ";
        }
    }
    cout << endl;

    nRet=select(nMaxFd+1, &fr, &fw,&fe,&tv); //Pause the program and tell me which sockets are ready.


    cout<<endl<<"After Select count : ";
    for (int i = 0; i <= nMaxFd; i++) {
        if (FD_ISSET(i, &fr)) {
            cout << i << " ";
        }
    }
    cout << endl;
}
    

    return 0;
}







