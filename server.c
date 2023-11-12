#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "execute_instruction.c"


void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// //  // //  // //  // //  // // //  // //  // //  // // IP V4 OR IP V6 // // //  // //  // //  // // 
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
// //  // //  // //  // //  // // //  // //  // //  // // ADDRINFO STRUCT SET TO NULL // // //  // //  // //  // // 
void null_addrinfo( struct addrinfo * s ){
    s->ai_flags = 0;                                                                // AI_PASSIVE, AI_CANONNAME, etc.                               
    s->ai_family = 0;                                                               // AF_INET, AF_INET6, AF_UNSPEC
    s->ai_socktype = 0;                                                             // SOCK_STREAM, SOCK_DGRAM
    s->ai_protocol = 0;                                                             // "any" if 0
    s->ai_addrlen = 0;                                                              // size_t of ai_addr in bytes
    s->ai_addr = NULL;                                                              // struct sockaddr_in or _in6
    s->ai_canonname = NULL;                                                         // full canonical hostname
    s->ai_next = NULL;                                                              // next node in linked list, type struct addrinfo*
}

// GET INFO FROM OPERATIVE SYSTEM AND WAIT FOR CONNECTIONS
//                                                    queue = how many pending connections can wait before error network busy try later
int epathy_listen( char* port , unsigned int queue ){ 
    int sockfd, new_fd;                                                             // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;                                           // store info from GETADDRINFO syscall , p will be just an alias
    struct sockaddr_storage foreign_addr;                                           // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    null_addrinfo(&hints);                                                          // set whole struct as 0
    hints.ai_family = AF_UNSPEC;                                                    // IPv4 or IPv6 possible
    hints.ai_socktype = SOCK_STREAM;                                                // TCP
    hints.ai_flags = AI_PASSIVE;                                                    // localhost or my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {                   // GETADDINFO SYSTEM CALL ()
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 0;
    }
    // loop through linked list from getaddrinfo syscall and bind to the first suitable found
    for(p = servinfo; p != NULL; p = p->ai_next) {
                                
        if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) { // SOCEKT sys call, get file descriptor ()
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) { // update file descriptor
            perror("setsockopt");
            return 0;
        }           
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {                        // BIND syscall ()
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;                                                                      // if everything good
    }
    freeaddrinfo(servinfo);                                                         // getaddrinfo output not needed anymore
    if (p == NULL) {                                                                // no SOCKET fd can BIND ()
        fprintf(stderr, "server: failed to bind\n");
        return 0;
    }
    if (listen(sockfd, queue) == -1) {                                              // LISTEN syscall to socket descriptor ()
        perror("listen");                                                           // LISTEN ERROR
        return 0;
    }
    sa.sa_handler = sigchld_handler;                                                // reap all dead processes
    sigemptyset(&sa.sa_mask);                                                       // set sigaction struct to NULL
    sa.sa_flags = SA_RESTART;                                                       // set restart flag
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {                                      // sigaction
        perror("sigaction");
        return 0;
    }

    // web framework
    unsigned int size;
    char result;
    char buffer[1024*1024];

    while (1){                                                                      // accepting, waiting connections
        sin_size = sizeof foreign_addr;
        new_fd = accept( sockfd , (struct sockaddr *)&foreign_addr , &sin_size );   // ACCEPT syscall in file descriptor for response ()
        if ( new_fd == -1 ) {
            perror("accept");                                                       // ACCEPT ERROR
            continue;
        }                       
        inet_ntop(  foreign_addr.ss_family,                                         // IPv4 or IPv6
                    get_in_addr((struct sockaddr *)&foreign_addr),                  
                    s,
                    sizeof s
                );
        
        printf("server: got connection from %s\n", s);

        // manage request
        result = recv(new_fd, buffer, (1024*1024), 0);
        
        if(result != -1)size = execute_instruction(buffer,(unsigned int) result);
        
        if (!fork()) {                                                              // this is the child process
            close(sockfd);                                                          // child doesn't need the listener
            if (send(new_fd, buffer, size, 0) == -1)
                perror("send");
                close(new_fd);
                exit(0);
        }
        close(new_fd);                                                              // parent doesn't need this
        //str_unset(buffer, size);
        buffer[0] = '\0';
    }
    return 1;
}
