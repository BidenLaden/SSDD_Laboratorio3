#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lines.h"
#include <arpa/inet.h>



int main(int argc, char *argv[])
{
        int sd;
	int val;
	int err;

        struct sockaddr_in server_addr, client_addr;
        socklen_t size;
        char buffer[256];
        int sc;
        //int n;


        sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sd < 0) {
                perror("Error in socket");
                exit(1);
        }

        val = 1;
        err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
        if (err < 0) {
                perror("Error in opction");
                exit(1);
        }
        setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

	bzero((char *)&server_addr, sizeof(server_addr));
    	server_addr.sin_family      = AF_INET;
    	server_addr.sin_addr.s_addr = INADDR_ANY;
    	server_addr.sin_port        = htons(4200);

	// Complete .....
        err = bind(sd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1) {
		printf("Error en bind\n");
		return -1;
	}

    	err = listen(sd, SOMAXCONN);
	if (err == -1) {
		printf("Error en listen\n");
		return -1;
	}
        size = sizeof(client_addr);

        while(1){
                printf("Esperando conexion\n");
                sc = accept(sd, (struct sockaddr *)&client_addr, (socklen_t *) &size);
                if(sc == -1){
                        printf("Error en el accept\n");
                        return(-1);
                }
                printf("Conexion aceptada de IP: %s y puerto: %d\n", 
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                //Ahora hacemos la parte que es "espejo" del cliente
                while(1){
                       err = readLine(sc, buffer, 256);
                       err = sendMessage(sc, buffer, strlen(buffer)+1);
                       if(strcmp(buffer, "EXIT") == 0){
                               break;
                       } 
                }       
        }

	
        close (sd);
        return(0);
}

