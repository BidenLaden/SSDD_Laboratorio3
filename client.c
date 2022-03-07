#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "lines.h"

#define MAX_LINE 	256

int main(int argc, char *argv[])
{
        int err; //Para el connect
        char buffer[MAX_LINE]; //para el readline
        //int n; //Para el readline
        //int sm; //Para el sendmessage
        //int rd; //Para el readline del servidor

        int sd;
        struct sockaddr_in server_addr;
        struct hostent *hp;

        if (argc != 3) {
                printf("Usage: client <serverAddress> <serverPort>\n");
                exit(0);
        }

	// the socket is create
        sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sd < 0){
                perror("Error in socket");
                exit(1);
        }

	// Obtain Server address 
        bzero((char *)&server_addr, sizeof(server_addr));
        hp = gethostbyname (argv[1]);
        if (hp == NULL) {
                perror("Error en gethostbyname");
                exit(1);
        }

	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[2]));

	// Complete.....
	// First: establish the connection

        err = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
        if(err == -1){
                printf("Error en el connect\n");
                return(-1);
        }

        //Leo del teclado, envio al servidor, leo del servidor e imprimo
        while(1){
                err = readLine(0, buffer, MAX_LINE); //Leemos del descriptor de salida 0 (es decir, por teclado)
                //Ahora envio al servidor con "sendmessage" (OJO: hay que enviar la longitud de la cadena con strlen +1 que es el caracter 0)
                err = sendMessage(sd, buffer, strlen(buffer)+1);
                //Ahora leo del servidor con readline
                err = readLine(sd, buffer, MAX_LINE);
                printf("%s\n", buffer);
                //Imprimo lo que leo del servidor
                if(strcmp(buffer, "EXIT") == 0){
                        break;
                }else{
                        printf("%s\n ", buffer);
                }

        }
        return (sd);

        return(0);
}

