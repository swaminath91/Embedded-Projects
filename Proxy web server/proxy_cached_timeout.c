/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#include <sys/time.h>
#include<semaphore.h>
#include <errno.h>

/* Default file permissions are DEF_MODE & ~DEF_UMASK */
/* $begin createmasks */
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH
/* $end createmasks */

/* Simplifies calls to bind(), connect(), and accept() */
/* $begin sockaddrdef */
typedef struct sockaddr SA;
/* $end sockaddrdef */

/* Persistent state for the robust I/O (Rio) package */
/* $begin rio_t */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;
/* $end rio_t */

#define DEBUGx
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif

/* External variables */
extern int h_errno;    /* defined by BIND for DNS errors */ 
extern char **environ; /* defined by libc */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
/* Misc constants */
#define	MAXLINE	 8192  /* max text line length */
#define MAXBUF   8192  /* max I/O buffer size */
#define LISTENQ  1024  /* second argument to listen() */

/* Our own error-handling functions */
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void app_error(char *msg);
/* functions */
void *thread(void *vargp);
void do_proxy(int fd);
void uri_parsing(char *uri, char *hostname, char *path, int *port);

struct hostent *gethostbyname_ts(char *hostname);
int open_clientfd_ts(char *hostname, int port);

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, 
	char *longmsg);
void reset(char *buffer1,char *buffer2, char * method, char *request,
 char *uri, char *hostname, char *path);
//char * site;
//int global_time;

/* Thread lock */
pthread_rwlock_t lock;

/* Mutex */
static sem_t mutex;
char url[4000];

#define MAX_CACHE_SIZE (1<<20) /* 1MB */
#define MAX_OBJECT_SIZE (100<<10) /* 100kb */

/* cache_object in linked list */
struct linkedlist_cache {
    char * data; /* HTML code */
	int size; /* size of the data */
	long int lru_time_track; /* least recently used */
	char * uri;	/* identify cache objects by uri */
	struct linkedlist_cache * next; 
};

typedef struct linkedlist_cache cache_object;


/* Thread lock */
pthread_rwlock_t lock;

int add_to_cache(char * data, int site_size, char* uri);
void remove_from_cache();
cache_object* cache_find(char * uri);





/* Head of linked list cache_objects */
cache_object * head  ;
int cache_size;
long int global_time;


/**
 * is_get_method - Check if its a get request
 * @param  method - Store request method
 * @return 0 - its not a get request, 1 - get request
 */
int is_get_method(char * method){
	if (strcasecmp(method, "GET")) { 
		return 0;
	}
	return 1;
}

/************************** 
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
void unix_error(char *msg) /* unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

void posix_error(int code, char *msg) /* posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void dns_error(char *msg) /* dns-style error */
{
    fprintf(stderr, "%s: DNS error %d\n", msg, h_errno);
    exit(0);
}

void app_error(char *msg) /* application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

/* $begin forkwrapper */
pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}
/* $end forkwrapper */

void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
    if (execve(filename, argv, envp) < 0)
	unix_error("Execve error");
}

/* $begin wait */
pid_t Wait(int *status) 
{
    pid_t pid;

    if ((pid  = wait(status)) < 0)
	unix_error("Wait error");
    return pid;
}
/* $end wait */

pid_t Waitpid(pid_t pid, int *iptr, int options) 
{
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0) 
	unix_error("Waitpid error");
    return(retpid);
}

/* $begin kill */
void Kill(pid_t pid, int signum) 
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
	unix_error("Kill error");
}
/* $end kill */

void Pause() 
{
    (void)pause();
    return;
}

unsigned int Sleep(unsigned int secs) 
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
	unix_error("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}
 
void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
	unix_error("Setpgid error");
    return;
}

pid_t Getpgrp(void) {
    return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions 
 ***********************************/

/* $begin sigaction */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}
/* $end sigaction */

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
	unix_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
	unix_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{ 
    if (sigfillset(set) < 0)
	unix_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
	unix_error("Sigaddset error");
    return;
}

void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
	unix_error("Sigdelset error");
    return;
}

int Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
	unix_error("Sigismember error");
    return rc;
}


/********************************
 * Wrappers for Unix I/O routines
 ********************************/

int Open(const char *pathname, int flags, mode_t mode) 
{
    int rc;

    if ((rc = open(pathname, flags, mode))  < 0)
	unix_error("Open error");
    return rc;
}

ssize_t Read(int fd, void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0) 
	unix_error("Read error");
    return rc;
}

ssize_t Write(int fd, const void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
	unix_error("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) 
{
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
	unix_error("Lseek error");
    return rc;
}

void Close(int fd) 
{
    int rc;

    if ((rc = close(fd)) < 0)
	unix_error("Close error");
}

int Select(int  n, fd_set *readfds, fd_set *writefds,
	   fd_set *exceptfds, struct timeval *timeout) 
{
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
	unix_error("Select error");
    return rc;
}

int Dup2(int fd1, int fd2) 
{
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
	unix_error("Dup2 error");
    return rc;
}

void Stat(const char *filename, struct stat *buf) 
{
    if (stat(filename, buf) < 0)
	unix_error("Stat error");
}

void Fstat(int fd, struct stat *buf) 
{
    if (fstat(fd, buf) < 0)
	unix_error("Fstat error");
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) 
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
	unix_error("mmap error");
    return(ptr);
}

void Munmap(void *start, size_t length) 
{
    if (munmap(start, length) < 0)
	unix_error("munmap error");
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size) 
{
    void *p;

    if ((p  = malloc(size)) == NULL)
	unix_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size) 
{
    void *p;

    if ((p  = realloc(ptr, size)) == NULL)
	unix_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size) 
{
    void *p;

    if ((p = calloc(nmemb, size)) == NULL)
	unix_error("Calloc error");
    return p;
}

void Free(void *ptr) 
{
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE *fp) 
{
    if (fclose(fp) != 0)
	unix_error("Fclose error");
}

FILE *Fdopen(int fd, const char *type) 
{
    FILE *fp;

    if ((fp = fdopen(fd, type)) == NULL)
	unix_error("Fdopen error");

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream) 
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
	app_error("Fgets error");

    return rptr;
}

FILE *Fopen(const char *filename, const char *mode) 
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
	unix_error("Fopen error");

    return fp;
}

void Fputs(const char *ptr, FILE *stream) 
{
    if (fputs(ptr, stream) == EOF)
	unix_error("Fputs error");
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream)) 
	unix_error("Fread error");
    return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    if (fwrite(ptr, size, nmemb, stream) < nmemb)
	unix_error("Fwrite error");
}


/**************************** 
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol) 
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
	unix_error("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) 
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
	unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) 
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
	unix_error("Bind error");
}

void Listen(int s, int backlog) 
{
    int rc;

    if ((rc = listen(s,  backlog)) < 0)
	unix_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
	unix_error("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) 
{
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
	unix_error("Connect error");
}

/************************
 * DNS interface wrappers 
 ***********************/

/* $begin gethostbyname */
struct hostent *Gethostbyname(const char *name) 
{
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL)
	dns_error("Gethostbyname error");
    return p;
}
/* $end gethostbyname */

struct hostent *Gethostbyaddr(const char *addr, int len, int type) 
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
	dns_error("Gethostbyaddr error");
    return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, 
		    void * (*routine)(void *), void *argp) 
{
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
	posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
    int rc;

    if ((rc = pthread_cancel(tid)) != 0)
	posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return) {
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0)
	posix_error(rc, "Pthread_join error");
}

/* $begin detach */
void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
	posix_error(rc, "Pthread_detach error");
}
/* $end detach */

void Pthread_exit(void *retval) {
    pthread_exit(retval);
}

pthread_t Pthread_self(void) {
    return pthread_self();
}
 
void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
    pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(sem_t *sem, int pshared, unsigned int value) 
{
    if (sem_init(sem, pshared, value) < 0)
	unix_error("Sem_init error");
}

void P(sem_t *sem) 
{
    if (sem_wait(sem) < 0)
	unix_error("P error");
}

void V(sem_t *sem) 
{
    if (sem_post(sem) < 0)
	unix_error("V error");
}

/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)
 */
/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
	    if (errno == EINTR) /* interrupted by sig handler return */
		nread = 0;      /* and call read() again */
	    else
		return -1;      /* errno set by read() */ 
	} 
	else if (nread == 0)
	    break;              /* EOF */
	nleft -= nread;
	bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}
/* $end rio_readn */

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nwritten = write(fd, bufp, nleft)) <= 0) {
	    if (errno == EINTR)  /* interrupted by sig handler return */
		nwritten = 0;    /* and call write() again */
	    else
		return -1;       /* errorno set by write() */
	}
	nleft -= nwritten;
	bufp += nwritten;
    }
    return n;
}
/* $end rio_writen */


/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) {  /* refill if buf is empty */
	rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
			   sizeof(rp->rio_buf));
	if (rp->rio_cnt < 0) {
	    if (errno != EINTR) /* interrupted by sig handler return */
		return -1;
	}
	else if (rp->rio_cnt == 0)  /* EOF */
	    return 0;
	else 
	    rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;          
    if (rp->rio_cnt < n)   
	cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}
/* $end rio_read */

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
/* $begin rio_readinitb */
void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}
/* $end rio_readinitb */

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
/* $begin rio_readnb */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
	if ((nread = rio_read(rp, bufp, nleft)) < 0) {
	    if (errno == EINTR) /* interrupted by sig handler return */
		nread = 0;      /* call read() again */
	    else
		return -1;      /* errno set by read() */ 
	} 
	else if (nread == 0)
	    break;              /* EOF */
	nleft -= nread;
	bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}
/* $end rio_readnb */

/* 
 * rio_readlineb - robustly read a text line (buffered)
 */
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) { 
	if ((rc = rio_read(rp, &c, 1)) == 1) {
	    *bufp++ = c;
	    if (c == '\n')
		break;
	} else if (rc == 0) {
	    if (n == 1)
		return 0; /* EOF, no data read */
	    else
		break;    /* EOF, some data was read */
	} else
	    return -1;	  /* error */
    }
    *bufp = 0;
    return n;
}
/* $end rio_readlineb */

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
    return n;
}


void Rio_writen(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
} 

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
    return rc;
} 

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - open connection to server at <hostname, port> 
 *   and return a socket descriptor ready for reading and writing.
 *   Returns -1 and sets errno on Unix error. 
 *   Returns -2 and sets h_errno on DNS (gethostbyname) error.
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, int port) 
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	return -1; /* check errno for cause of error */

    /* Fill in the server's IP address and port */
    if ((hp = gethostbyname(hostname)) == NULL)
	return -2; /* check h_errno for cause of error */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], 
	  (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    /* Establish a connection with the server */
    if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
	return -1;
    return clientfd;
}
/* $end open_clientfd */

/*  
 * open_listenfd - open and return a listening socket on port
 *     Returns -1 and sets errno on Unix error.
 */
/* $begin open_listenfd */
int open_listenfd(int port) 
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
  
    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	return -1;
 
    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
		   (const void *)&optval , sizeof(int)) < 0)
	return -1;

    /* Listenfd will be an endpoint for all requests to port
       on any IP address for this host */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port); 
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
	return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
	return -1;
    return listenfd;
}
/* $end open_listenfd */

/******************************************
 * Wrappers for the client/server helper routines 
 ******************************************/
int Open_clientfd(char *hostname, int port) 
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0) {
	if (rc == -1)
	    unix_error("Open_clientfd Unix error");
	else        
	    dns_error("Open_clientfd DNS error");
    }
    return rc;
}

int Open_listenfd(int port) 
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
	unix_error("Open_listenfd error");
    return rc;
}

int add_to_cache(char * data, int site_size, char* uri) {
	if (site_size + sizeof(cache_object) <= MAX_OBJECT_SIZE)
	{
		return 0;
	} else {
		dbg_printf("add_to_cache");
		/* Writing to the cache - only one thread should be able to write to it */
		pthread_rwlock_wrlock(&lock); /* lock on the resource*/
		/* Free enough space for the new site in cache*/
		while(cache_size + site_size + sizeof(cache_object) >= MAX_CACHE_SIZE ){
			remove_from_cache();
		}
		/* Add the site node to the start of the cache list*/
		cache_object * site = (cache_object *)malloc(sizeof(cache_object));
		site -> next = head;
		site -> data = data;
		site -> lru_time_track = global_time++;
		site -> size = site_size;
		site -> uri = malloc(( strlen( uri ) + 1 ) * sizeof( char ));
		strcpy( site -> uri, uri );
		head = site;
		cache_size +=  sizeof( cache_object ) + site_size + 
						strlen( uri ) + 1;

		pthread_rwlock_unlock( &lock );
		return 1;
	}
}


/*
	cache_find - returns cache_object if found, otherwise NULL.
*/

cache_object* cache_find( char * uri ){
	dbg_printf( "cache_find" );
	//cache_object * site = NULL;
cache_object * site;
    printf("INSIDE Cache_find %s\n",uri);
//head != NULL;
	if( head != NULL ){
		printf( "here" );
		for ( site = head; site != NULL ; 
			site = site -> next ){
			printf("here 2\n");
			printf("%s\n",site ->uri );
			printf("%s\n",uri );
			if (!strcmp( site -> uri, uri ))
			{
				printf("Cache hittt\n");
				break;
			}			
		}
	} else {
		printf(" Cache miss\n ");
		return NULL;
	}
	if(site != NULL)
		printf(" Cache Hit\n ");
	return site;
}


/*
	remove_from_cache - add a cache_object to the cache
*/
void remove_from_cache(){
	cache_object * p ;  	/* Previous pointer */
	cache_object * q ;		/* Current Pointer */
	cache_object * temp ;	/* pointer to free */

	if( head != NULL) {
		/* find lru */
		/* traverse the list */
		for (q = head, p = head, temp =head ; q -> next != NULL; 
			q = q -> next) {
			if(( (q -> next) -> lru_time_track) < (temp -> lru_time_track)) {
				temp = q -> next;
				p = q;
			}
		}

		/* free required node from the list*/
		if(temp == head) {
			head = head -> next; /*Handle the base case*/
		} else {
			p->next = temp->next;	
		}

		/* Change cache size(free temp node) */
		cache_size = cache_size - (temp -> size) - sizeof(cache_object) - 
			strlen(temp -> uri) - 1;

		/* Free temp node*/
		free(temp->data);
		free(temp->uri);
		free(temp);
	} 
	return;
}

char data1[50000];int timeout;
int main(int argc, char **argv) 
{
	int listenfd, port;
	int *connfdp;
	global_time =0;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	pthread_t tid;
	pthread_rwlock_init(&lock, 0);
    Sem_init(&mutex,0,1);
	cache_size = 0;

	/* Check command line args */
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);
printf("\n enter the timeout ");
        scanf("%d",&	timeout);
        printf("\n timeout is %d\n",timeout);

	/* Ingore SIGPIPE */
	signal(SIGPIPE, SIG_IGN);
	listenfd = open_listenfd(port);

	while (1) {
		connfdp = Malloc(sizeof(int));
		if(connfdp == NULL){
			continue;
		}
		*connfdp = accept(listenfd, (SA *)&clientaddr, &clientlen);

		/* Create thread and let thread handle functions */
		if(( *connfdp > 0 )){
			Pthread_create(&tid, NULL, thread, connfdp);
		}
	}
	Free(connfdp);
}


/**
 * thread - Handles concurrent request from the client
 * @param vargp - Connection file descriptor received when connection was
 *                accepted from the client
 */
void *thread(void *vargp) {
	int connfd = *((int *) vargp);
	pthread_detach(pthread_self());
	Free(vargp);
	do_proxy(connfd);
	close(connfd);
	return NULL;
}


/**
 * do_proxy - Function handles the Http request from the client. It searches 
 * 			  for the request in the proxy cache, if not available in cache,
 * 			  retreives from the web server. 
 * @param clientfd - Conection file descriptor sent to the function by the
 *                 	 thread
 */
void do_proxy(int clientfd) {
	int serverfd, port, content_len, content_size = 0,count = 0;

	char *buffer1 = Calloc(MAXLINE,1); /* reads http request from the client*/
	char *buffer2 = Calloc(MAXLINE,1); /* reads response from the server*/
	char *uri = Calloc(MAXLINE,1); /* Resource Identifier */
	char *path = Calloc(MAXLINE,1); /* Relative path to be extracted from the 
		 							  uri*/
	char *method = Calloc(MAXLINE,1); /* GET Method*/
	char *request = Calloc(MAXLINE,1); /* Request from proxy(1.0) */
	char *hostname = Calloc(MAXLINE,1); /*Hostname - to be extracted from 
		 							    the uri*/
		 
	char* data = NULL; 
	//cache_object * site = NULL;
cache_object * site;
//cacheobj * site = NULL;
	rio_t server_rio;
	rio_t client_rio;




	/* Read request line and headers */
	rio_readinitb(&client_rio, clientfd);

	/* Read each line of the header request*/
	while(rio_readlineb(&client_rio, buffer1, MAXLINE) && 
		strcmp(buffer1, "\r\n")){
		/*if first line, retrieve method and uri */
		dbg_printf("---------------------- %d\n", count);
		/* Count can have 0 or 1 value*/
		if(!count){
		    dbg_printf("Reading each line of the header request: %s",buffer1);
			sscanf(buffer1, "%s %s", method, uri);
			/* Parses the uri */
			uri_parsing(uri, hostname, path, &port);
			sprintf(buffer1, "%s %s HTTP/1.0\r\n", method, path);
			strcat(request, buffer1);
			count =1;
		} else {
			/*request is comprised of all lines of header */
			char * word;
			dbg_printf("FILTER :%s",buffer1);
			
			/* change from HTTP/1.1 to HTTP/1.0 */
			if((word = strstr(buffer1, "HTTP/1.1"))){
				strncpy(word, "HTTP/1.0", strlen("HTTP/1.0"));
			}
			
			/* change to Proxy-Connection:close */
			if((word = strstr(buffer1, "Proxy-Connection: "))){
				strcpy(buffer1, "Proxy-Connection: close\r\n");
			} else if((word = strstr(buffer1, "Connection:"))){
				strcpy(buffer1, "Connection: close\r\n");
			}
			
			/* Fail safe - delete Keep-Alive */
			if((word = strstr(buffer1, "Keep-Alive:"))){
				buffer1 = "";
			}
			dbg_printf("FILTERAfter :%s",buffer1);
			strcat(request, buffer1);
//printf("\n buffer1 is %s",buffer1);
		}

	}
	
	dbg_printf("\nDEBUG: %s",request);
	
	/* Add new line at the end of request*/
	strcat(request, "\r\n");

	/* Only handle GET */
	if( !is_get_method( method )) {
		reset(buffer1,buffer2,method,request,uri,hostname,path);
		clienterror(clientfd, method, "501", "Not Implemented", 
			"Proxy does	not implement this.");
		return;
	}

char *p;
p = strstr(url,uri);
p = strtok(p,"\n");

if (p)
{
printf("\n url is in cache: %s\n", p);
//rio_writen(clientfd, data1, strlen(data1));
send(clientfd,data1, strlen(data1),0);
sleep(10);
memset(p,0,strlen(p));
memset(data1,0,strlen(data1));
}
else
{
	 strcat(url,uri);
         strcat(url,"\n");
printf("\n url is not in cache\n"); 
             //  printf("\n cache url is %s",url);	



	pthread_rwlock_rdlock(&lock);
	/* Find uri in cached and write to clientfd */
	if((site = cache_find(uri)) != NULL){
              
printf("this is a test printf");
		rio_writen(clientfd, site->data, site->size); 	
		site -> lru_time_track = global_time++;
		printf("Cache hit\n");
		pthread_rwlock_unlock(&lock);
		reset(buffer1,buffer2,method,request,uri,hostname,path);
		return;
	}
else
{
printf("this is a else printf");
}
	pthread_rwlock_unlock(&lock);

	if((serverfd = open_clientfd_ts(hostname, port)) == -1){
		printf("Open Serverfd failed.\n");		
		reset(buffer1,buffer2,method,request,uri,hostname,path);
		return;
	}

	rio_readinitb(&server_rio, serverfd); 

	
	if(rio_writen( serverfd, request, strlen( request )) < 0){
		close(serverfd);
		reset(buffer1,buffer2,method,request,uri,hostname,path);
		return;
	}

	/* Read response from Server, send to client and add to cache */
	while((content_len = rio_readnb(&server_rio, buffer2, MAXLINE)) > 0){
               strcpy(data1,buffer2);
		rio_writen(clientfd, buffer2, content_len);
		//printf("\n buffer2 is %s",buffer2);
		if((data = Realloc(data, content_size + 
			content_len*sizeof(char))) == NULL){
			printf("Realloc error\n");
			close(serverfd);
			reset(buffer1,buffer2,method,request,uri,hostname,path);
			return;
		}
		
		memcpy(data + content_size, buffer2, content_len);
		
		content_size = content_size + content_len;
printf("data + content is %s", data);
printf("\n url again is %s\n", uri);
add_to_cache(data, content_size, uri);
//printf("\n data cached is site",site);
	}		

	//add_to_cache(data, content_size, uri);

	if(!add_to_cache(data, content_size, uri)){
		free(data);
printf("\n data is freed");
	}	
else
{
add_to_cache(data, content_size, uri);
printf("\n data cached is %s\n",uri);
	}

}
	close(serverfd);
	reset(buffer1,buffer2,method,request,uri,hostname,path);

	return;
}


void uri_parsing(char *uri, char *hostname, char *path, int *port) {	
	dbg_printf("uri_parsing");
	char *temp;
	*port = 0;
	strcpy(path, "/");
	if((temp = strpbrk(strpbrk(uri, ":") + 1, ":"))){
		sscanf(temp, "%d %s", port, path);
		sscanf(uri, "http://%[^:]s", hostname);
	} else {

		sscanf(uri, "http://%[^/]s/", hostname);
		sscanf(uri, "http://%*[^/]%s", path);
	}
	if((*port) == 0){
		*port = 80;
	}

}




/*
    Thread Safety
	open_clientfd_ts - calls thread safe gethostbyname_ts
*/
int open_clientfd_ts(char *hostname, int port)
{
 	/* From csapp */

    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}

	/* Use thread-safe gethostbyname_ts */

    if ((hp = gethostbyname_ts(hostname)) == NULL){
	  return -2; /* check h_errno for cause of error */
	} 
    
	bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;    
    bcopy((char *)hp->h_addr_list[0], 
    (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    
	serveraddr.sin_port = htons(port);

    /* Establish a connection with the server */
  	if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0){
		return -1;
	}
    return clientfd;
}

/*
	clienterror - returns an error message to the client
*/
void clienterror(int fd, char *cause, char *errnum, 
	char *shortmsg, char *longmsg) 
{
	/* From csapp */

	char buffer1[MAXLINE], body[MAXBUF];

	/* Build the HTTP response body */
	sprintf(body, "<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

	/* Print the HTTP response */
	sprintf(buffer1, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	Rio_writen(fd, buffer1, strlen(buffer1));
	sprintf(buffer1, "Content-type: text/html\r\n");
	Rio_writen(fd, buffer1, strlen(buffer1));
	sprintf(buffer1, "Content-length: %d\r\n\r\n", (int)strlen(body));
	Rio_writen(fd, buffer1, strlen(buffer1));
	Rio_writen(fd, body, strlen(body));
}


/*
 	gethostbyname_ts - threadsafe version of gethostbyname
*/ 

struct hostent *gethostbyname_ts(char *hostname) {
  	struct hostent *host_ent;
  	/* Make it atomic instruction*/
  	P(&mutex);
  		host_ent = Gethostbyname(hostname);
  	V(&mutex);

	return host_ent;
}


/*
	reset - handles freeing of do_proxy
*/
void reset(char *buffer1,char *buffer2, char * method, char *request, 
	char *uri, char *hostname, char *path){
//sleep(10);
//printf("\n 10 seconds is over ");
	free(buffer1); 
	free(buffer2); 
	free(method); 
	free(request); 
	free(uri); 
        //free(p); 
	free(hostname); 
	free(path);
   //   memset(url,0,strlen(url));
}

    


