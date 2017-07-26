/*
 * Simple TCP Port scanner
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#define h_addr h_addr_list[0]

int main(int argc, char **argv) {
    
    if (argc < 3) {
      fprintf(stderr,"usage %s 'hostname' 'start-port' optional-'end-port'\n", argv[0]);
      exit(1);
    }

    int sockfd, startPort, endPort;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *rhost;

    rhost = argv[1];

    startPort = atoi(argv[2]);

    if(argv[3] == NULL){
        endPort = startPort;
    } else {
    endPort = atoi(argv[3]);
    }
    if (endPort < startPort){
        fprintf(stderr, "Invalid port range!\n");
        exit(1);
    }
    
    printf("HOST: %s\n",rhost);
    printf("START PORT: %i\n",startPort);
    printf("END PORT: %i\n",endPort);

    for(int i = startPort; i <= endPort; i++){
        
        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        
        if (sockfd < 0) {
            perror("ERROR opening socket");
            exit(1);
        }
        
        server = gethostbyname(rhost);
       
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            break;
        }
       
        memset(&serv_addr,0 , sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
        serv_addr.sin_port = htons(i);

        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
          fprintf(stderr, "HOST: [ %s ] PORT: %i [ CLOSED ]\n", rhost, i);

        } else {
         fprintf(stdout, "HOST: [ %s ] PORT: %i [ OPEN ]\n", rhost, i);
        }
   close(sockfd);
   }
   return 0;
}
