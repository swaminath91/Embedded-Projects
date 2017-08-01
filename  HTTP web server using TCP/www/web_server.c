/*Header files initialization */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
#include <errno.h>
/* *********************************************** */

/* constants definition */
#define CONNMAX 1000
#define BYTES 1024
#define SA      struct sockaddr
#define MAXLINE 4096
#define MAXSUB  200
#define TRUE   1
#define FALSE  0
#define PORT23 8888

/* Global variables declaration */
char *ROOT; //variable for storing the ROOT directory of the files
char *PORT2;

/* character pointers for storing the data types supported in a structure */
char *STRUCT1,*STRUCT2,*STRUCT3,*STRUCT4,*STRUCT5,*STRUCT6,*STRUCT7,*STRUCT8,*STRUCT9,*STRUCT10,*STRUCT11,*STRUCT12;
char *STRUCT13,*STRUCT14,*STRUCT15,*STRUCT16,*STRUCT17,*STRUCT18,*STRUCT19,*STRUCT20;
char STRUCT1_1[20],STRUCT2_1[20],STRUCT3_1[20],STRUCT4_1[20],STRUCT5_1[20],STRUCT6_1[20],STRUCT7_1[20],STRUCT8_1[20],STRUCT9_1[20],STRUCT10_1[20];
char STRUCT11_1[20],STRUCT12_1[20],STRUCT13_1[20],STRUCT14_1[20],STRUCT15_1[20],STRUCT16_1[20],STRUCT17_1[20],STRUCT18_1[20],STRUCT19_1[20];
char Connection[50];
char Timeout_value[50];
int activity,max_sd;

/* timer struct variable for checking the timeout period */
struct timeval tv;
fd_set readfds;
int listenfd, clients[CONNMAX];
void error(char *);
int message_size=0;
 char *message, response[4096];
extern int h_errno;

/* structure definition for storing the data types supported from the config file */
 typedef struct {
 char *ext;
 char *mediatype;
} extn;
int k1;


/* I read the extensions from the config file and store in a structure, comments about the content type are just for my reference */	
extn extensions[] ={
 {STRUCT1_1, "text/html" },//html
 {STRUCT2_1, "text/htm"  },//html
 {STRUCT3_1, "text/plain" },//txt
 {STRUCT4_1,"image/png"},//png
 {STRUCT5_1, "image/gif" },//gif
 {STRUCT6_1, "image/jpg" },//jpg
 {STRUCT7_1, "text/css" },//css
 {STRUCT8_1,  "text/javascript"  }, //javascript
 {STRUCT9_1, "image/x-icon" }, //x-icon
 {STRUCT10_1, "image/zip" },//zip
 {STRUCT11_1, "image/gz"},  //gz
 {STRUCT12_1, "image/tar" },//tar
 {STRUCT13_1, "text/html" },//php
 {STRUCT14_1,"image/ppm"},//ppm
 {STRUCT15_1,"image/jpeg"},//jpeg
 {STRUCT16_1,"application/pdf"}, //pdf
 {STRUCT17_1, "application/octet-stream"},//rar
 {STRUCT18_1, "audio/mp3"},//mp3
 {STRUCT19_1, "video/mkv"},//mkv
 {0,0} }; 

/* starting server */
void Server_pgm(char *port)  
{
    struct addrinfo server, *res, *p;

    /* getaddrinfo for host */
    memset (&server, 0, sizeof(server));//clearing the server array
    server.ai_family = AF_INET;
    server.ai_socktype = SOCK_STREAM;  //TCP connection
    server.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &server, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }

    /* to socket and bind */
    for (p = res; p!=NULL; p=p->ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break; //bind the socket
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    /* to listen for incoming connections */
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}


/* Function for processing the POST request */
ssize_t http_process(int sockfd, char *host, char *page, char *poststr)
{

	char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
	ssize_t n;

	snprintf(sendline, MAXSUB,
		 "POST %s HTTP/1.0\r\n"
		 "Host: %s\r\n"
		 "Content-type: application/x-www-form-urlencoded\r\n"
		 "Content-length: %d\r\n\r\n"
		 "%s", page, host, strlen(poststr), poststr);

/* printing the request details on the terminal */
printf("POST %s HTTP/1.0\r\n Host: %s\r\n Content-type: application/x-www-form-urlencoded\r\n Content-length: %d\r\n\r\n %s",page,host,strlen(poststr),poststr); 
		 
	write(sockfd, sendline, strlen(sendline)); //write to the socket, the details about the request
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = '\0';
		printf("%s", recvline);
	}
	return n;

}

/* function for reading the file size of the requested data types*/
int get_file_size(int fd) {
 struct stat stat_struct;
 if (fstat(fd, &stat_struct) == -1)
  return (1);
 return (int) stat_struct.st_size;
}

/* client connection */
void client_request(int n)
{
/* local variables for storing the message, data-to-send and request */
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd=recv(clients[n], mesg, 99999, 0);

    if (rcvd<0)    // receive error
  {
        fprintf(stderr,("recv() error\n"));
}
    else if (rcvd==0)    // receive socket closed
{
sleep(10);
      fprintf(stderr,"Client disconnected unexpectedly.\n");
} 

 /* message received */
   else   
    {
        printf("%s", mesg);
        reqline[0] = strtok (mesg, " \t\n"); //using string token to filter out tabs and new line character

/* if request is GET */
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");

/* If it's not a valid request, print 400, bad request */
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.0 400 Bad Request</H1></body></html>\n", 47); //FILE NOT FOUND
		printf("\nHTTP/1.0 400 Bad Request\n");  
            }

/* if valid request, process it */
            else
            {
                if ( strncmp(reqline[1], "/\0", 2)==0 )
                    reqline[1] = "/index.html";        //if no file is specified, index.html will be opened by default

/* storing the path from config file into path char array */
                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);
   char* s = strchr(reqline[1], '.');

   int i;

/* if the user requested extensions match with the data types in the config file */
   for (i = 0; extensions[i].ext != NULL; i++) {
    if (strcmp(s + 1, extensions[i].ext) == 0) {


                if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND, process it and send the data requested
                {
/* sending data with header information */
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (clients[n], data_to_send, bytes_read);

 		printf(" HTTP/1.0 200 OK, File found\n"); 
 printf("Content-Type: %s\n",
        extensions[i].mediatype);

   int length = get_file_size(fd);
printf("Content length is %d bytes\n",length);
printf("\n");
                }

/* otherwise send, the file is not found with 404 error */
                else    
		{
/* if the file is not found, send the error information as a text */
		send(clients[n], "HTTP/1.0 404 Not Found\n\n", 23, 0);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.0 404 Not Found</H1></body></html>\n", 47); //FILE NOT FOUND
                printf(" HTTP/1.0 404 File not found\n");     
		printf("\n");
                }
break;
}

 int size = sizeof(extensions) / sizeof(extensions[0]);

/* if the data type is not valid or not present in the config file, return 501 error saying not implemented */
    if (i == size - 2) {
     printf("501 Unsupported Media Type\n");
   send(clients[n], "HTTP/1.0 501 unsupported\n\n", 23, 0);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.1 501 Unsupported</H1></body></html>\n", 45); 
             
}
}

            }
        }

   }

    //Closing SOCKET
    shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clients[n]);
    clients[n]=-1;
}

/* client_pipeline connection */
void client_pipeline(int n)
{
/* local variables for storing the message, data-to-send and request */
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd=recv(clients[n], mesg, 99999, 0);

    if (rcvd<0)    // receive error
  {
        fprintf(stderr,("recv() error\n"));
}
    else if (rcvd==0)    // receive socket closed
{
sleep(10);

        fprintf(stderr,"Client disconnected unexpectedly.\n");
} 

 /* message received */
   else   
    {
        printf("%s", mesg);
        reqline[0] = strtok (mesg, " \t\n");//using string token to filter out tabs and new line character

/* if request is GET */
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {

            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");

/* If it's not a valid request, print 400, bad request */
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.0 400 Bad Request</H1></body></html>\n", 47); //FILE NOT FOUND
		printf("\nHTTP/1.0 400 Bad Request\n");  
            }

/* if valid request, process it */
            else
            {
                if ( strncmp(reqline[1], "/\0", 2)==0 )
                    reqline[1] = "/index.html";   //if no file is specified, index.html will be opened by default

/* storing the path from config file into path char array */
                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);
   char* s = strchr(reqline[1], '.');

   int i;

/* if the user requested extensions match with the data types in the config file */
   for (i = 0; extensions[i].ext != NULL; i++) {
    if (strcmp(s + 1, extensions[i].ext) == 0) {


                if ( (fd=open(path, O_RDONLY))!=-1 )     //FILE FOUND, process it and send the data requested
                {
/* sending data with header information */
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (clients[n], data_to_send, bytes_read);
 		printf(" HTTP/1.0 200 OK, File found\n"); 
 printf("Content-Type: %s\n",
        extensions[i].mediatype);

   int length = get_file_size(fd);
printf("Content length is %d bytes\n",length);
printf("Connection: %s\n",Connection); 

/* check for whether the connection type is keep-alive */
if(strcmp(Connection,"Keepalive")==0)
{
sleep(10);
}
printf("\n");
// wait until either socket has data ready to be recv()d (timeout 10 secs)
tv.tv_sec = 10; //initialize the value read from config file into the timeval struct variable

/* use select function to handle timeout period */
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &tv);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

/* if there's no activity or any request for 10 seconds, print a timeout on the server */
else if (activity == 0) {
    printf("Timeout occurred!  No data after 10 seconds.\n");
exit(0);
}
                }

/* otherwise send, the file is not found with 404 error */
                else    
		{
/* if the file is not found, send the error information as a text */
		send(clients[n], "HTTP/1.0 404 Not Found\n\n", 23, 0);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.0 404 Not Found</H1></body></html>\n", 47); //FILE NOT FOUND
                printf(" HTTP/1.0 404 File not found\n");     
		printf("\n");
/* check for whether the connection type is keep-alive */
if(strcmp(Connection,"Keepalive")==0)
{
sleep(10);
}
printf("\n");
// wait until either socket has data ready to be recv()d (timeout 10 secs)
tv.tv_sec = 10; //initialize the value read from config file into the timeval struct variable

/* use select function to handle timeout period */
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &tv);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

/* if there's no activity or any request for 10 seconds, print a timeout on the server */
else if (activity == 0) {
    printf("Timeout occurred!  No data after 10 seconds.\n");
exit(0);
}
                }
break;
}

 int size = sizeof(extensions) / sizeof(extensions[0]);

/* if the data type is not valid or not present in the config file, return 501 error saying not implemented */
    if (i == size - 2) {
     printf("501 Unsupported Media Type\n");
   send(clients[n], "HTTP/1.0 501 unsupported\n\n", 23, 0);
		write(clients[n], "HTTP/1.1 200 OK\n", 16);
		write(clients[n], "Content-length: 46\n", 19);
		write(clients[n], "Content-Type: text/html\n\n", 25);
		write(clients[n], "<html><body><H1>HTTP/1.1 501 Unsupported</H1></body></html>\n", 45); 
/* check for whether the connection type is keep-alive */
if(strcmp(Connection,"Keepalive")==0)
{
sleep(10);
}
printf("\n");
// wait until either socket has data ready to be recv()d (timeout 10 secs)
tv.tv_sec = 10; //initialize the value read from config file into the timeval struct variable

/* use select function to handle timeout period */
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &tv);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

/* if there's no activity or any request for 10 seconds, print a timeout on the server */
else if (activity == 0) {
    printf("Timeout occurred!  No data after 10 seconds.\n");
exit(0);
}
             
}
}

            }
        }

   }

    //Closing SOCKET
    shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clients[n]);
    clients[n]=-1;
}


/* main function, arguments not mandatory */
int main(int argc, char* argv[])
{
int k;

printf("enter k = ");
scanf("%d", &k);

/* to check for GET-pipeline for webserver*/
if(k == 1)
{
/* socket variables initialization for GET */
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    char PORT[10];


/* Reading the configuration file */
FILE* fp11 = fopen("ws_conf", "rb"); 
        if(fp11==NULL)
        {
            printf("Config file not found, so server can't start\n");
            return 1;
        }

/* ******************** Reading the file location ******************* */
char buff[1000];  int end, loop, line; char *PORT1;
    char str[10];
 fgets(buff, 1000, fp11);

 char* nl_ptr = strrchr(buff, '\n');
 if (nl_ptr != NULL)
  *nl_ptr = '\0'; 
printf("\n buffer is %s\n",buff);//printing the buffer location
ROOT = buff;
/* ****************************************************************** */
  
/* *****************Reading the PORT Number from config file*********************************** */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){//include '\n'
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
//        printf("\nLine-%d: %s\n", line, str); 
PORT1 = strtok(str, " ");
PORT1 = strtok(PORT1, "\n");
   strcpy(PORT,PORT1);
/* ****************************************************************** */



/******************** Structure values ***************************************************** */

/*********** .html ************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){//include '\n'
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT1 = strtok(PORT1, "\n");
strcpy(STRUCT1_1,STRUCT1);
/* ***************************************************** */

/* ********************* htm *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){//include '\n'
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT2 = strtok(PORT1, "\n");
strcpy(STRUCT2_1,STRUCT2);
/* ***************************************************** */

/* ********************* txt *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT3 = strtok(PORT1, "\n");
strcpy(STRUCT3_1,STRUCT3);
/* ***************************************************** */

/* ********************* png *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
    
PORT1 = strtok(str, " ");
STRUCT4 = strtok(PORT1, "\n");
strcpy(STRUCT4_1,STRUCT4);
/* ***************************************************** */

/* *********************gif *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){//include '\n'
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT5 = strtok(PORT1, "\n");
strcpy(STRUCT5_1,STRUCT5);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT6 = strtok(PORT1, "\n");
strcpy(STRUCT6_1,STRUCT6);
/* ***************************************************** */
/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT7 = strtok(PORT1, "\n");
strcpy(STRUCT7_1,STRUCT7);
/* ***************************************************** */
/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
STRUCT8 = strtok(PORT1, "\n");
strcpy(STRUCT8_1,STRUCT8);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT9 = strtok(PORT1, "\n");
strcpy(STRUCT9_1,STRUCT9);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)
    
PORT1 = strtok(str, " ");
STRUCT10 = strtok(PORT1, "\n");
strcpy(STRUCT10_1,STRUCT10);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT11 = strtok(PORT1, "\n");
strcpy(STRUCT11_1,STRUCT11);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end) 
PORT1 = strtok(str, " ");
STRUCT12 = strtok(PORT1, "\n");

strcpy(STRUCT12_1,STRUCT12);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
 
PORT1 = strtok(str, " ");
STRUCT13 = strtok(PORT1, "\n");
strcpy(STRUCT13_1,STRUCT13);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT14 = strtok(PORT1, "\n");
strcpy(STRUCT14_1,STRUCT14);
/* ***************************************************** */
/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT15 = strtok(PORT1, "\n");
strcpy(STRUCT15_1,STRUCT15);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
STRUCT16 = strtok(PORT1, "\n");
strcpy(STRUCT16_1,STRUCT16);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
   
PORT1 = strtok(str, " ");
STRUCT17 = strtok(PORT1, "\n");
strcpy(STRUCT17_1,STRUCT17);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
 
PORT1 = strtok(str, " ");
STRUCT18 = strtok(PORT1, "\n");
strcpy(STRUCT18_1,STRUCT18);
/* ***************************************************** */

/* ********************* *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT19 = strtok(PORT1, "\n");
strcpy(STRUCT19_1,STRUCT19);
/* ***************************************************** */


/* ****************Connection type ********************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)
PORT2 = strtok(str, " ");
STRUCT20 = strtok(PORT2, "\n");
strcpy(Connection,STRUCT20);
printf("Connection: %s\n",Connection);
/* ****************************************************** */

 fclose(fp11); //closing the file configuration read
    int slot=0;

    //Parsing the command line arguments
    while ((c = getopt (argc, argv, "p:r:")) != -1)
        switch (c)
        {
            case 'r':
                ROOT = malloc(strlen(optarg));
                strcpy(ROOT,optarg);
                break;
            case 'p':
                strcpy(PORT,optarg);
                break;
            case '?':
                fprintf(stderr,"Wrong arguments given!!!\n");
                exit(1);
            default:
                exit(1);
        }
    
    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    Server_pgm(PORT); //calling the server to respond to HTTP request

    // ACCEPT connections
   while (1)
{ 

        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);//accepting connections
//sleep(10);
        if (clients[slot]<0)    //if there are negative request, print error
            error ("accept() error");
        else                       //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
                client_pipeline(slot);
                exit(0);
            }
        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }






}

/* ******************Function for POST************************************** */
if(k == 2)
{
int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	/* ****************************************************** */
        char *hname = "localhost";  //host
	char *page = "/login.html"; //html file 
	char *poststr = "user= &pw= &action= login\r\n";//sample post data format
	/* ****************************************************** */


	char str[50];
	struct hostent *hptr;
	if ((hptr = gethostbyname(hname)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s",
			hname, hstrerror(h_errno));
		exit(1);
	}

/* printing the host name and address */
	printf("hostname: %s\n", hptr->h_name);
	if (hptr->h_addrtype == AF_INET
	    && (pptr = hptr->h_addr_list) != NULL) {
		printf("address: %s\n",
		       inet_ntop(hptr->h_addrtype, *pptr, str,
				 sizeof(str)));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80); //port number 80 for apache server
	inet_pton(AF_INET, str, &servaddr.sin_addr);

	connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
http_process(sockfd, hname, page, poststr); //call the function and pass some parameters
close(sockfd);
	//exit(0);
}
/* **********************************POST********************************************* */


/*Function for pipeline in echoserver */
if(k == 3)
{
/*local variables */
 int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
struct timeval tv; //timeval struct variable for storing the timeout value
    struct sockaddr_in address;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readfds;
      
    //message
    char *message = "ECHO Daemon v1.0 \r\n";
  
    //to initialize all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //to create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //to set master socket to allow multiple connections 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created with port number
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT23);
      
    //binding the socket to localhost port
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT23);//printing the port number 
     
    //to try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //to accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
     
    while(TRUE) 
    {
        //to clear the socket set
        FD_ZERO(&readfds);
  
        //to add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //to add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor, then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            if(sd > max_sd)
                max_sd = sd;
        }
  
// wait until either socket has data ready to be recv()d (timeout 10 secs)
tv.tv_sec = 10;//timeout value for the timer

/* to use select function to handle timeout period */
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &tv);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

/* if there's no activity or any request for 10 seconds, print a timeout on the server */
else if (activity == 0) {
    printf("Timeout occurred!  No data after 10 seconds.\n");
} 
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //to inform user of socket number,used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //to send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            puts("Welcome message sent successfully");
              
            //to add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                   
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    close( sd );
                    client_socket[i] = 0;
                }
                  
                //Echo back the message received
                else
                {
                    //to set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
      






}


/* *******************************GET* ********************************************** */
/* socket variables initialization for GET */
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    char PORT[10];

/* Reading the configuration file */
FILE* fp11 = fopen("ws_conf", "rb"); 
        if(fp11==NULL)
        {
            printf("Config file not found, so server can't start\n");
            return 1;
        }

/* ******************** Reading the file location ******************* */
char buff[1000];  int end, loop, line; char *PORT1;
    char str[10];
 fgets(buff, 1000, fp11);

 char* nl_ptr = strrchr(buff, '\n');
 if (nl_ptr != NULL)
  *nl_ptr = '\0'; 
printf("\n buffer is %s\n",buff); //printing the buffer location
ROOT = buff;
/* ****************************************************************** */
  
/* ***************** Reading the PORT Number from config file*********************************** */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
PORT1 = strtok(PORT1, "\n");
strcpy(PORT,PORT1);
/* ****************************************************************** */



/******************** Reading data types from the config file***************************************************** */

/*********** .html ************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT1 = strtok(PORT1, "\n");
strcpy(STRUCT1_1,STRUCT1);
/* ***************************************************** */

/* **********************htm**************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT2 = strtok(PORT1, "\n");
strcpy(STRUCT2_1,STRUCT2);
/* ***************************************************** */

/* ********************* txt *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT3 = strtok(PORT1, "\n");
strcpy(STRUCT3_1,STRUCT3);
/* ***************************************************** */

/* ********************* png *************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
    
PORT1 = strtok(str, " ");
STRUCT4 = strtok(PORT1, "\n");
strcpy(STRUCT4_1,STRUCT4);
/* ***************************************************** */

/* *********************gif***************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT5 = strtok(PORT1, "\n");
strcpy(STRUCT5_1,STRUCT5);
/* ***************************************************** */

/* ***************************************************** */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT6 = strtok(PORT1, "\n");
strcpy(STRUCT6_1,STRUCT6);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT7 = strtok(PORT1, "\n");
strcpy(STRUCT7_1,STRUCT7);
/* ***************************************************** */

/* ************************************************ */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
STRUCT8 = strtok(PORT1, "\n");
strcpy(STRUCT8_1,STRUCT8);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT9 = strtok(PORT1, "\n");
strcpy(STRUCT9_1,STRUCT9);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)
    
PORT1 = strtok(str, " ");
STRUCT10 = strtok(PORT1, "\n");
strcpy(STRUCT10_1,STRUCT10);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT11 = strtok(PORT1, "\n");
strcpy(STRUCT11_1,STRUCT11);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end) 
PORT1 = strtok(str, " ");
STRUCT12 = strtok(PORT1, "\n");

strcpy(STRUCT12_1,STRUCT12);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
 
PORT1 = strtok(str, " ");
STRUCT13 = strtok(PORT1, "\n");
strcpy(STRUCT13_1,STRUCT13);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT14 = strtok(PORT1, "\n");
strcpy(STRUCT14_1,STRUCT14);
/* ***************************************************** */
/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT15 = strtok(PORT1, "\n");
strcpy(STRUCT15_1,STRUCT15);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
STRUCT16 = strtok(PORT1, "\n");
strcpy(STRUCT16_1,STRUCT16);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
   
PORT1 = strtok(str, " ");
STRUCT17 = strtok(PORT1, "\n");
strcpy(STRUCT17_1,STRUCT17);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
     
       end = 1;
            break;
        }
    }
    if(!end)
 
PORT1 = strtok(str, " ");
STRUCT18 = strtok(PORT1, "\n");
strcpy(STRUCT18_1,STRUCT18);
/* ***************************************************** */

/* ************************************************* */
end = 0; loop =0; str[0]='\0';
for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
       end = 1;
            break;
        }
    }
    if(!end)
PORT1 = strtok(str, " ");
STRUCT19 = strtok(PORT1, "\n");
strcpy(STRUCT19_1,STRUCT19);
/* ***************************************************** */

 fclose(fp11);
    int slot=0;

    /* Parsing the command line arguments */
    while ((c = getopt (argc, argv, "p:r:")) != -1)
        switch (c)
        {
            case 'r':
                ROOT = malloc(strlen(optarg));
                strcpy(ROOT,optarg);
                break;
            case 'p':
                strcpy(PORT,optarg);
                break;
            case '?':
                fprintf(stderr,"Wrong arguments given!!!\n");
                exit(1);
            default:
                exit(1);
        }
    
    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    Server_pgm(PORT); //calling the server to respond to HTTP request

    /* ACCEPT connections */
   while (1)
{ 

        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen); //accepting connections

        if (clients[slot]<0)              //if there are negative request, print error
            error ("accept() error");
        else                        //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
                client_request(slot);
                exit(0);
            }
        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }

    return 0;
}




