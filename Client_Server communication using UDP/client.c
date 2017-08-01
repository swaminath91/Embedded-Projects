/*Header files declaration*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

/*Variables for struct timer */
unsigned long int RTT;
static struct timespec rtclk_start_time = {0, 0};
static struct timespec rtclk_stop_time = {0, 0};

  int i,ack=0,p=0,q=0;
  
  struct timespec rtclk_resolution;

/*Function for comparing the size of the remaining buffer which are sent in chunks*/
int minimum(int x, int y)
{
int a,b;
a=x; b=y;
if(x<y)
{
return x;
}
else
{
return y;
}

}

/*Main function with two arguments-ip address and the port number */
int main(int argc, char * argv[]){

/*Variables for client server socket addressing */ 
  int clientSocket, portNum, nBytes,nBytes1,nBytes2,nBytes_ls,o=0;
  char *buffer=NULL;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

/*Buffer declaration variables */
int n=0,nread,bytesReceived,bytesReceived1;
char readbuf_client[45000];
char readbuf_client_ls[45000];
char writebuf_client1[45000];
char writebuf_client_1[45000];
char recvBuff11[45000];

/*For checking the number of arguments at the client end at run time*/
if (argc < 3)
	{
		printf("USAGE:  <server_ip> <server_port>\n");
		exit(1);
	}

/*Creating a socket at the client end*/
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(atoi(argv[2]));//assigning the ip address of the host
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);//assigning any port number above 5000
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  //clearing the address variable
  addr_size = sizeof serverAddr;//reading the address size

while(1) //while(1) characher infinite loop starting
{
/*variables for file name and command parsing and storage */
char * pch;
char * pch1;

/*variables for count declaration of number of bytes sent or received*/
nread = 0;
 nBytes1=0;nBytes2 = 0;readbuf_client[0] = '\0';

/*command line allowing the user to enter any of the file commands*/
printf("\n get for requesting a file from the server");
printf("\n put for sending a file to the server");
printf("\n ls for reading the contents of the server directory and sending them to the client");
printf("\n exit for exiting the process ");
printf("\nEnter the command + file_name ");

 int sz=0;
 getline(&buffer,&sz,stdin);//reading the user input
nBytes = strlen(buffer) + 1; //checking the size of the buffer
    
    
sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size); //sending the command and the file name to the server

pch = strtok (buffer," ");//using string token to remove out blank spaces that might be added
n++;

while (n<2)
{
pch1 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (pch1==NULL)
{
break;
}
else
{
n++;
}
}

/*command for put- client to server*/
if(strcmp(pch,"put")==0)
{
/*buffer initialization*/
char buffer_size2[4000];
char seq_num[2000];

int n=0,n1=0; int bytesToWrite;
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added
printf("\nCommand is %s\n", pch);

/*opening the file in read mode*/
FILE *input = fopen(pch1,"rb");
        if(input==NULL)
        {
            printf("File open error");
            return 1;   
        } 

 fseek(input, 0, SEEK_END);
bytesToWrite = ftell(input);//reading the file size to be sent to the server
printf("bytes to write is %d",bytesToWrite);


int bytes=(int)bytesToWrite;
sprintf(buffer_size2,"%d",bytes);//storing the file size in a string
puts(buffer_size2);
  sendto(clientSocket,buffer_size2,strlen(buffer_size2),0,(struct sockaddr *)&serverAddr,addr_size);//sending the file size to the server   

fclose(input);//closing the file

/*opening the file again in read mode*/
FILE* fp = fopen(pch1,"rb");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;   
        }

int bytesWritten=0; 

/*comparing for whether the all the contents of the file to be sent to the server are read */
while(bytesWritten<bytesToWrite)
    {
        int m=minimum(256,bytesToWrite-bytesWritten);
        unsigned char *buff = malloc(sizeof(char)*m);//creating a dynamic buffer to send every chunks of data
        bzero(buff, m);
        int nread = fread(buff,1,m,fp);
        printf("\nBytes read %d \n", nread);  
       
sprintf(seq_num,"%d",n1);//writing the sequence number in a string
puts(buffer_size2);

        if(nread > 0)
        {
            printf("Sending ");
clock_gettime(CLOCK_REALTIME, &rtclk_start_time);/*starting the structure timer for calculating the RTT for reliability*/
sendto(clientSocket,buff,nread,0,(struct sockaddr *)&serverAddr,addr_size); //sending the chunks of data to the server
sendto(clientSocket,seq_num,2000,0,(struct sockaddr *)&serverAddr,addr_size); //sending the sequence number along with that

printf("\nPacket %d is sent\n",n1);

int nBytes2 = recvfrom(clientSocket,readbuf_client,45000,0,(struct sockaddr *)&serverAddr, &addr_size);//receiving the acknowledgement from the server
ack++;
n = atoi(readbuf_client);//ascii to integer conversion for checking the acknowledgement
printf("Acknowledgement receieved %s %d\n",readbuf_client,n);
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);//stopping the timer to calculate RTT

if(n1 == n)//sequence number and the acknowledgement are verified for sending the next packet
{
n1++;
  bytesWritten+=m;  
}

/*printing the RTT for the process */
 printf("RT clock start seconds = %ld, nanoseconds = %ld\n", 
         rtclk_start_time.tv_sec, rtclk_start_time.tv_nsec);

  printf("RT clock stop seconds = %ld, nanoseconds = %ld\n", 
         rtclk_stop_time.tv_sec, rtclk_stop_time.tv_nsec); 
RTT = rtclk_stop_time.tv_nsec-rtclk_start_time.tv_nsec;
printf("RTT is %ld nanoseconds\n",RTT);

/*if RTT is greater than 10ms, timeout */
if(RTT > 10000000)
{
 bytesWritten -= m;//resend the packet
o -= 1;
}

}       

/*if the chunk contents are read, report end of the chunk*/
        if (nread < 256)
        {
            if (feof(fp))
                printf("End of file\n");
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
memset(seq_num,0,strlen(seq_num));//clearing the sequence number in the string
    }


/*clearing the variables for next iteration */
memset(buffer_size2,0,strlen(buffer_size2));
memset(writebuf_client1,0,strlen(writebuf_client1));
bytesToWrite = 0;
nread = 0;
nBytes1=0;o=1;bytesWritten = 0; 
fclose(fp);//closing the file
}

/*command for get- server to client*/
else if(strcmp(pch,"get")==0)
{

/*buffer initialization*/
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added

/*receiving the number of bytes of the file that will be sent by the server and necessarily allocation buffer in chunks for that size */
recvfrom(clientSocket,recvBuff11,5000,0,(struct sockaddr *)&serverStorage, &addr_size);

int bytesToRead1 = atoi(recvBuff11);//ascii to integer conversion for storing the buffer size in integer

   printf("Bytes read 1 %d \n", bytesToRead1);

/*opening file in write mode to store the contents sent by the server*/
FILE* fp11 = fopen("client_get", "w+"); 
        if(fp11==NULL)
        {
            printf("Error opening file");
            return 1;
        }
     
       
        int totalbytesread1=0;
   printf("Bytes read 1 %d \n", bytesToRead1);

/*comparing for whether the all the contents of the file sent by the server are read */
        while(totalbytesread1 < bytesToRead1)
        {
            int m1 = minimum(256,bytesToRead1-totalbytesread1);
            char *recvBuff1=malloc(sizeof(char)*m1);//creating a dynamic buffer to store every chunks of data
            bzero(recvBuff1,m1);
  bytesReceived1 = recvfrom(clientSocket,recvBuff1,m1,0,(struct sockaddr *)&serverStorage, &addr_size);//reading the packets of data from the server
        
            printf("Bytes received %d\n",bytesReceived1);
recvfrom(clientSocket,seq_num1,2000,0,(struct sockaddr *)&serverStorage, &addr_size);//receiving the sequence number sent by the server for the packet

int Seqnum1 = atoi(seq_num1);//converting the string to integer for the sequence number received

/*checking for whether all the contents of the packet are received and the correct sequence number as well*/
if((bytesReceived1 == m1)&&(Seqnum1 == p))
{
printf("Packet received\n");

  sendto(clientSocket,seq_num1,2000,0,(struct sockaddr *)&serverAddr,addr_size); //if all perfect,send an acknowledgement to the server
p++; 
printf("Sending Acknowledgement\n");
 totalbytesread1 += bytesReceived1;   //add the bytes received to store in a file
  fwrite(recvBuff1, 1,bytesReceived1,fp11);//write the data in a file to be stored at client local directory
}
else
{
printf("Packet %d is lost\n",p);//if sequence number and the packet sent is not received, report a loss
}
           
}

//if you haven't received any data, report a read error 
        if(bytesReceived1 < 0)
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
p = 0;
memset(seq_num1,0,strlen(seq_num1));
memset(recvBuff11,0,strlen(recvBuff11));

nread = 0; writebuf_client1[0] = '\0'; nBytes1=0; bytesReceived1=0;totalbytesread1=0;bytesToRead1=0;
fclose(fp11);//closing the file

}

/*command ls-for reading the contents of the server directory by the server and server sending the info the client*/
else if(strcmp(pch,"ls")==0)
{
/*buffer initialization*/
char recvBuff12[5000];char seq_num2[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added

/*receiving the number of bytes of the file to be received from the server and necessarily allocation buffer in chunks for that size */
recvfrom(clientSocket,recvBuff12,5000,0,(struct sockaddr *)&serverStorage, &addr_size);

int bytesToRead1 = atoi(recvBuff12);//ascii to integer conversion for storing the buffer size in integer

   printf("Bytes read 1 %d \n", bytesToRead1);

/*opening file in write mode to store the contents sent by the server*/
FILE* fp11 = fopen("client_server_ls.txt", "w+"); 
        if(fp11==NULL)
        {
            printf("Error opening file");
            return 1;
        }
    
        
        int totalbytesread1=0;
   printf("Bytes read 1 %d \n", bytesToRead1);

/*comparing for whether the all the contents of the file sent by the server are read */
        while(totalbytesread1 < bytesToRead1)
        {
            int m1 = minimum(256,bytesToRead1-totalbytesread1);
            char *recvBuff1=malloc(sizeof(char)*m1);//creating a dynamic buffer to store every chunks of data
            bzero(recvBuff1,m1);
  bytesReceived1 = recvfrom(clientSocket,recvBuff1,m1,0,(struct sockaddr *)&serverStorage, &addr_size);//reading the packets of data from the server
         
            printf("Bytes received %d\n",bytesReceived1);

recvfrom(clientSocket,seq_num2,2000,0,(struct sockaddr *)&serverStorage, &addr_size);//receiving the sequence number sent by the server for the packet

int Seqnum2 = atoi(seq_num2);//converting the string to integer for the sequence number received

/*checking for whether all the contents of the packet are received and the correct sequence number as well*/
if((bytesReceived1 == m1)&&(Seqnum2 == q))
{
printf("Packet received\n");

  sendto(clientSocket,seq_num2,2000,0,(struct sockaddr *)&serverAddr,addr_size); //if all perfect,send an acknowledgement to the server
q++; 
printf("Sending Acknowledgement\n");
            totalbytesread1 += bytesReceived1; //add the bytes received to store in a file  
  fwrite(recvBuff1, 1,bytesReceived1,fp11);//write the data in a file to be stored at client local directory
}
else
{
printf("Packet %d is lost\n",q);//if sequence number and the packet sent is not received, report a loss
}
    }

//if you haven't received any data, report a read error 
        if(bytesReceived1 < 0)
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
q = 0;
memset(seq_num2,0,strlen(seq_num2));
memset(writebuf_client1,0,strlen(writebuf_client1));
memset(recvBuff12,0,strlen(recvBuff12));
nread = 0; writebuf_client1[0] = '\0'; nBytes1=0;
fclose(fp11);//closing the file

}

/*commad exit for exiting the process*/
else if(strcmp(pch,"exit")==0)
{
exit(0);
}

/*otherwise report invalid command*/
else
{
printf("\n Invalid command \n");
}

}//while(1) ending
return 0;
}
 
