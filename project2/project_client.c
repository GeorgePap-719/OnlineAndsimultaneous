// giwrgos papadopoulos

#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h> 
#include <time.h>
#include <sys/wait.h> //for wait function
#include <pthread.h>
#include <string.h>
#include <sys/types.h> /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/un.h> /* for Unix domain sockets */
#include <sys/ipc.h>
#include <assert.h>
#include<arpa/inet.h> //inet_addr
//stathera gia to onoma tou socket
#define UNIXSTR_PATH "/tmp/socket.str"

//Number of clients
#define LISTENQ 50

#define SOCKET_NAME "A"
//defines
#define Tseatfind 8 //seconds
#define Tcardcheck 3 //seconds
#define Npelates 100
int op=0;
int temporary=10;
int PassingNumbers[3] = {0};
time_t t;
 
int main (int argc , char **arfv){
	
 srand((unsigned) time(&t));
 int SocketDesc;
 struct sockaddr_un ServerAddr; /* Struct for the server socket address. */
int zone,Number_of_tickets,credit_card;

	/* Intializes random number generator */
	zone = rand()%100+1;
	Number_of_tickets=rand()%5+1;
	credit_card = rand()%100+1;
	//printf("zone:%d \ntickets:%d\ncard:%d\n",zone,Number_of_tickets,credit_card);
	/* Starting Client */
	printf("\nstarting client\n");
    SocketDesc = socket( AF_LOCAL, SOCK_STREAM, 0 ); // Create the client's endpoint
    bzero( &ServerAddr, sizeof( ServerAddr ) ); // Zero all fields of ServerAddr
    ServerAddr.sun_family = AF_LOCAL; // Socket type is local (Unix Domain)
    strcpy( ServerAddr.sun_path, UNIXSTR_PATH ); // Define the name of this socket
    //Connect the client's and the server's endpoint
    connect(SocketDesc, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr));
	//Sending Data
	write(SocketDesc, &Number_of_tickets, sizeof(int));
	write(SocketDesc, &zone, sizeof(int));
	write(SocketDesc, &credit_card, sizeof(int));
	

	return 0;
}


















