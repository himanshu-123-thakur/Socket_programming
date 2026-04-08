#include <iostream>
#include <cstring>
#include <cstdlib>

/*

<cstdlib>        → For exit(), general utilities

<sys/types.h>    → Defines system data types like socklen_t, ssize_t

<sys/socket.h>   → Core socket APIs:
                   socket(), bind(), listen(), accept(), send(), recv()

<netinet/in.h>   → Defines Internet address structures:
                   sockaddr_in (IP + port), INADDR_ANY

<arpa/inet.h>    → Byte order + IP conversion:
                   htons(), htonl(), ntohs(), inet_addr()
                   (IMPORTANT: converts little endian → big endian for network)

<unistd.h>       → OS-level file descriptor operations:
                   close(), read(), write()
                   (socket is treated as a file descriptor)
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

struct sockaddr_in srv; ///* Structure describing an Internet socket address.  */
                        // alr defined

#define PORT 9909

int main(){

    int nRet=0;

    //initialise socket

    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //socketID

    /*| Family     | Meaning                  |
      | ---------- | ------------------------ |
      | `AF_INET`  | IPv4 (32-bit)            |
      | `AF_INET6` | IPv6 (128-bit)           |
      | `AF_UNIX`  | Local IPC (same machine) |
*/

    if(nSocket<0){
        perror("Socket Perror ");
        cout<<"Socket Not opened"<<endl;
    }
    else{
        cout<<"Socket opened => "<<nSocket<<endl;
    }





    //initialise the environment for sockaddr structure

    srv.sin_addr.s_addr = INADDR_ANY; // Used if my one machine as server (INADDR_ANY)
                                      //INADDR_ANY -> collect my ip address auto
                                      // else write ->  inet_addr ("127.0.0.1");
                                      // inet convert   ip to byte order
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    memset(&srv.sin_zero, 0, 8);

    cout<<INADDR_ANY<<endl;
    //BIND socket to local port
    nRet=bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
    if(nRet<0){
        cout<<endl<<"FAILED TO Bind to local port"<<endl;
        exit(EXIT_FAILURE);
    }
    else{
        cout<<"Successfully Bind to local port"<<endl;
    }


    //Listen the request from client
    nRet = listen(nSocket, 5); //5 means max 5 req in a active queue if more they wait
    if(nRet<0){
        cout<<"Fail to start Listen through local Port"<<endl;
        exit(EXIT_FAILURE);
    }
    else{
        cout<<"Started Listening"<<endl;
    }
    return 0;
}