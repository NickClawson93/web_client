#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

using namespace std;

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    
    /*
    if(argc < 3)
      {
        printf("\nUsage: download host-name host-port host-path \n");
        return 0;
      }
    else
      {
        strcpy(strHostName,argv[1]);
        nHostPort=atoi(argv[2]);
      }

    */
    extern char *optarg;
    int c, times_to_download = 1, err = 0;
    bool debug = false;
    while( (c = getopt( argc, argv, "c:d" ) ) != -1){
        switch(c){
            case 'c':
                times_to_download = atoi( optarg );
                break;
            case 'd':
                debug = true;
                break;
            case '?':
                err = 1;
                break;
        }
    }
    string path = argv[ optind + 2 ];
    string host = argv[ optind ];
    int port = atoi( argv[ optind + 1 ] );

    printf("\nMaking a socket");
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }
    printf("\nmade 1");
    /* get IP address from name */
    pHostInfo=gethostbyname(host.c_str());
    printf("\nmade 2");
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(port);
    Address.sin_family=AF_INET;

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
    printf("\nmade 3");

    string request = "GET / HTTP/1.1 \r\nHost: mclement.us\r\n\r\n";
    printf("\nmade 4");
    
    printf("\nWriting:\n%s",request.c_str());
    printf("\nmade 5");
    write(hSocket,request.c_str(),request.length());
    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
    printf("\nReceived \"%s\" from server\n",pBuffer);
    /* write what we received back to the server */

    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
}
