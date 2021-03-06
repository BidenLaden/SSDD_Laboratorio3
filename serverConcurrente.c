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
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 3
pthread_mutex_t m;
pthread_cond_t c;
int busy = false;

void tratar_peticion(int *sc){
    int s_local;
    char buffer[256];

    pthread_mutex_lock(&m);
    s_local = *sc;
    busy = true;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);

    //Si queremos que cada hilo al cerrarse diga quien se ha cerrado: getpeername() 
    //Ahora hacemos la parte que es "espejo" del cliente
    while(1){
        readLine(s_local, buffer, 256);
        sendMessage(s_local, buffer, strlen(buffer)+1);
        if(strcmp(buffer, "EXIT") == 0){
            break;
        } 
    } 
    close(s_local);
    pthread_exit(NULL);
}



int main(int argc, char *argv[]){

    int sd;
    int val;
    int err;

    struct sockaddr_in server_addr, client_addr;
    socklen_t size;
    int sc;
        
    pthread_attr_t attr;
    pthread_t thid;

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


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
        //busy == false inicialmente
        sc = accept(sd, (struct sockaddr *)&client_addr, (socklen_t *) &size);
        if(sc == -1){
            printf("Error en el accept\n");
            return(-1);
        }
            if(pthread_create(&thid, &attr, tratar_peticion, (void *)&sc) == 0){ 
                
                printf("Conexion aceptada de IP: %s y puerto: %d\n", 
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                pthread_mutex_lock(&m);
                while(busy == false){  
                    pthread_cond_wait(&c, &m);                        
                }     
                    busy = false;
                    pthread_mutex_unlock(&m);
            }else{
                printf("Error al crear el thread");
                return(-1);
            }
        
         
    }

	
        close (sd);
        return(0);
}

