/*
 * Simple TCP Port scanner
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define h_addr h_addr_list[0]

#define COLOR_RESET	"\x1b[0m"
#define LIGHT_RED			"\x1b[91m"
#define LIGHT_GREEN			"\x1b[92m"

struct args{
	char *rhost;
	char *startPort;
	char *endPort;
	int show_closed;
};

void usage(void);
struct args *parse_args(int argc, char **argv);

int main(int argc, char **argv) {
    
    struct args *Args = parse_args(argc, argv);
	
	if( (Args->rhost == NULL || Args->startPort == NULL) ){
		usage();
	}
	
    int sockfd, startPort, endPort, show_closed;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *rhost;

    rhost = Args->rhost;
    startPort = atoi(Args->startPort);
	show_closed = Args->show_closed;
    if(Args->endPort == NULL){
        endPort = startPort;
    } else {
    endPort = atoi(Args->endPort);
    }
    
    if (endPort < startPort){
        fprintf(stderr, "Invalid port range!\n");
        exit(1);
    }
    
    free(Args);
    
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
          if(show_closed != 1){
			fprintf(stderr, "HOST: [ %s ] PORT: %i [%s CLOSED %s]\n", rhost, i, LIGHT_RED, COLOR_RESET);
			}
        } else {
         fprintf(stdout, "HOST: [ %s ] PORT: %i [%s OPEN %s]\n", rhost, i, LIGHT_GREEN, COLOR_RESET);
        }
   close(sockfd);
   }
   return 0;
}

void usage(){
      fprintf(stderr, "Usage: tscan -r \"Remote host\" -p \"Port\" [-e] [end Port] [-o shows only open ports]\n");
      exit(1);
    }

struct args *parse_args(int argc, char **argv){
	
	struct args *arg = calloc(1, sizeof(struct args));
	int opt;
	int i = 0;
    while ((opt = getopt(argc, argv, "pero")) != -1) {
		i = i + 2;
        switch (opt) {
        case 'p': arg->startPort = argv[i]; break;
        case 'e': arg->endPort = argv[i]; break;
        case 'r': arg->rhost = argv[i]; break;
        case 'o': 
			arg->show_closed = 1; 
			i--;
			break;
        default:
            usage();
            
        }
        
    }
	
	return arg;
}
