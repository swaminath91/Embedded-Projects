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


/* Structure declaration for the variables to be used for the timer for reliability */
unsigned long int RTT;
static struct timespec rtclk_start_time = {0, 0};
static struct timespec rtclk_stop_time = {0, 0};
int i,o=0,p=0,r=0;
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

 int udpSocket, nBytes,nBytes3,o=0;

/*Buffer declaration variables */
  char buffer[65000];
char readbuf[65000];
char writebuf[65000];
char writebuf_server[65000];
char writebuf_server2[65000];
char writebuf_ls[65000];

/*Variables for client server socket addressing */ 
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;

/*variables for count declaration of number of bytes sent or received*/
  int i,n=0,bytesReceived,bytesReceived2; 

/*variables for file name and command parsing and storage */
char * pch;
char * pch1;

/*Variables for opening and reading the contents of the server directory*/
DIR *d;
struct dirent *dir;

int nread;

/*For checking the number of arguments at the server end at run time*/
if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}

/*Creating a socket at the server end*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(atoi(argv[1])); //assigning the ip address of the client
  serverAddr.sin_addr.s_addr = INADDR_ANY;//assigning any port number in accordance to the client
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);//clearing the address variable


  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//binding the socket

  addr_size = sizeof serverStorage; //reading the address size

/*zeroing the contents of buffers before alocation */
bzero(buffer,sizeof(buffer));   
bzero(buffer,sizeof(readbuf));

while(1)  //while(1) characher infinite loop starting
{
nread =0; writebuf[0] ='\0';nBytes3 = 0;readbuf[0] = '\0';//clearing the buffers and variables for next iteration
recvfrom(udpSocket,buffer,sizeof(buffer),0,(struct sockaddr *)&serverStorage, &addr_size);//reading the command and filename and storing in buffer

printf("\n buffer -  %s\n",buffer); //printing the buffer
pch = strtok (buffer," "); //using string token to separate command and filename
n++;
while (n<2)
{
pch1 = strtok (NULL, " ");//using string token again to remove out new line characters
if (pch1==NULL)
{
break;
}
else
{
n++;
}
}

/*function for put-client to server */
if(strcmp(pch,"put")==0)
{
/*buffer allocation */
char buffer_size1[5000]; char seq_num[2000];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added

/*receiving the number of bytes of the file to be sent and necessarily allocation buffer in chunks for that size */
recvfrom(udpSocket,buffer_size1,5000,0,(struct sockaddr *)&serverStorage, &addr_size);
puts(buffer_size1);
int bytesToRead = atoi(buffer_size1); //ascii to integer conversion for storing the buffer size in integer

   printf("Bytes read  %d \n", bytesToRead);

/*opening file in write mode to store the contents sent by the client*/
FILE *fp;
        fp = fopen("server_put", "wb"); 
        if(NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
       
        int totalbytesread=0;


/*comparing for whether the all the contents of the file sent by the client are read */
        while(totalbytesread < bytesToRead)
        {
            int m = minimum(256,bytesToRead-totalbytesread);
            char *recvBuff=malloc(sizeof(char)*m);//creating a dynamic buffer to store every chunks of data
            bzero(recvBuff,m);

/*starting the structure timer for calculating the RTT for reliability*/
clock_gettime(CLOCK_REALTIME, &rtclk_start_time);

  bytesReceived = recvfrom(udpSocket,recvBuff,m,0,(struct sockaddr *)&serverStorage, &addr_size);//reading the packets of data from the client
            printf("\nBytes received %d\n",bytesReceived);
recvfrom(udpSocket,seq_num,2000,0,(struct sockaddr *)&serverStorage, &addr_size);//receiving the sequence number sent by the client for the packet
int Seqnum = atoi(seq_num);//converting the string to integer for the sequence number received

/*checking for whether all the contents of the packet are received and the correct sequence number as well*/
if((bytesReceived == m)&&(Seqnum == o))
{
printf("Packet received\n");

  sendto(udpSocket,seq_num,2000,0,(struct sockaddr *)&serverStorage,addr_size); //if all perfect,send an acknowledgement to the client
o++; 
printf("Sending Acknowledgement\n");
 totalbytesread += bytesReceived;   //add the bytes received to store in a file
  fwrite(recvBuff, 1,bytesReceived,fp);//write the data in a file to be stored at server local directory
}
else
{
printf("Packet %d is lost\n",o); //if sequence number and the packet sent is not received, report a loss
}
  clock_gettime(CLOCK_REALTIME, &rtclk_stop_time); //end the timer for calculating the timer to store the file
 
 }

        if(bytesReceived < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
o = 0;
memset(buffer_size1,0,strlen(buffer_size1));
memset(seq_num,0,strlen(seq_num));

nread = 0; writebuf_server2[0] = '\0';bytesReceived=0; bytesToRead=0;totalbytesread=0;
fclose(fp);//closing the file
}

/*function for get- server to client*/
else if(strcmp(pch,"get")==0)
{
/*buffer initialization*/
char buffer_size[4000];
int bytesToWrite2=0,n=0,n2=0;
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);

/*opening file in read mode to send the contents of server to the client*/
FILE *input2 = fopen(pch1,"rb");
        if(input2==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(input2, 0, SEEK_END);
bytesToWrite2 = ftell(input2);//reading the size of the file 
printf("bytes to write is %d",bytesToWrite2);

int bytes=(int)bytesToWrite2;
sprintf(buffer_size,"%d",bytes);//writing the size of the file in bytes to the client
puts(buffer_size);
  sendto(udpSocket,buffer_size,strlen(buffer_size),0,(struct sockaddr *)&serverStorage,addr_size); //sending the file size of the file to be sent to the client

fclose(input2);//closing the file

/*opening the file in read mode*/
FILE* fp2 = fopen(pch1,"rb");
        if(fp2==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten2=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */
while(bytesWritten2<bytesToWrite2)
    {
        int m2=minimum(256,bytesToWrite2-bytesWritten2);
   
        unsigned char *buff_get2 = malloc(sizeof(char)*m2);//creating a dynamic buffer to send every chunks of data
        bzero(buff_get2, m2);
        int nread = fread(buff_get2,1,m2,fp2);//reading the number of bytes sent
        printf("\nBytes read %d \n", nread);  
         
sprintf(seq_num1,"%d",n2);//writing the sequence number in a string

        if(nread > 0)
        {
            printf("Sending");

/*starting the structure timer for calculating the RTT for reliability*/
clock_gettime(CLOCK_REALTIME, &rtclk_start_time);
  
  sendto(udpSocket,buff_get2,nread,0,(struct sockaddr *)&serverStorage,addr_size); //sending the chunks of data to the client
 
 sendto(udpSocket,seq_num1,2000,0,(struct sockaddr *)&serverStorage,addr_size);//sending the sequence number along with that

printf("\nPacket %d is sent\n",n2);

int nBytes2 = recvfrom(udpSocket,readbuf,45000,0,(struct sockaddr *)&serverStorage, &addr_size);//receiving the acknowledgement from the client

n = atoi(readbuf); //ascii to integer conversion for checking the acknowledgement
printf("Acknowledgement receieved %s %d\n",readbuf,n);


clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);//stoping the timer to calculate RTT

if(n2 == n) //sequence number and the acknowledgement are verified for sending the next packet
{
n2++;
bytesWritten2+=m2;  
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
 bytesWritten2 -= m2;//resend the packet
p -= 1;
} 
}       

/*if the chunk contents are read, report end of the chunk*/
        if (nread < 256) 
        {
            if (feof(fp2))
                printf("End of file\n");
            if (ferror(fp2))
                printf("Error reading\n");
            break;
        }
memset(seq_num1,0,strlen(seq_num1));//clearing the sequence number in the string

    }

/*clearing the variables for next iteration */
memset(writebuf_server2,0,strlen(writebuf_server2));
memset(readbuf,0,strlen(readbuf));
memset(buffer_size,0,strlen(buffer_size));
nread = 0; writebuf_server2[0] = '\0';
readbuf[0] = '\0';p=1;bytesWritten2 = 0;n2=0;n=0;RTT=0;
bytesToWrite2 = 0;
fclose(fp2);//closing the file
}

else if(strcmp(pch,"ls")==0)
{
/*directory variable initialization*/
  DIR *d;
  struct dirent *dir;
/*variables for reading the buffer and data packets */
int bytes=0,bytesToWrite3=0,n3=0;
char storage[256]; char seq_num3[2000];
char buffer_size4[5000];

/*opening the file in write mode to write all the contents of the server directory*/
  FILE *fp4 = fopen("ls_server_contents.txt","w");
        if(fp4==NULL)
        {
            printf("File open error");
            return 1;   
        }  
  d = opendir(".");//open the current directory
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      printf("%s\n", dir->d_name);
fprintf(fp4, "%s\n",dir->d_name); //file write the contents
    }
fclose(fp4);//close the file

/*opening the file in read mode*/
FILE* input1 = fopen("ls_server_contents.txt", "r+");
        if(input1==NULL)
        {
            printf("\nFile open error\n");
           return 1;
        }
	
fseek(input1, 0, SEEK_END);
bytesToWrite3 = ftell(input1);//read the file size to be sent to the client for buffer allocation
printf("\nbytes to write is %d",bytesToWrite3);	


int bytes=(int)bytesToWrite3;
sprintf(buffer_size4,"%d",bytes);//write the size of the file to be sent to the client

puts(buffer_size4);

  sendto(udpSocket,buffer_size4,strlen(buffer_size4),0,(struct sockaddr *)&serverStorage,addr_size); //sending the file size to the client

fclose(input1);//closing the file

/*opening the file again in read mode*/
FILE* input = fopen("ls_server_contents.txt", "r+");
        if(input==NULL)
        {
            printf("\nFile open error\n");
            return 1;
        }

int bytesWritten3=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */
while(bytesWritten3<bytesToWrite3)
    {
        int m3=minimum(256,bytesToWrite3-bytesWritten3);
     
        unsigned char *buff_ls = malloc(sizeof(char)*m3);//creating a dynamic buffer to store every chunks of data
        bzero(buff_ls, m3);
        int nread = fread(buff_ls,1,m3,input);
        printf("\nBytes read %d \n", nread);  
    
sprintf(seq_num3,"%d",n3);
 if(nread > 0)
            {
                printf("Sending: %s\n",buff_ls);

/*starting the structure timer for calculating the RTT for reliability*/
              clock_gettime(CLOCK_REALTIME, &rtclk_start_time);

sendto(udpSocket,buff_ls,nread,0,(struct sockaddr *)&serverStorage,addr_size);//sending the chunks of data to the client
 sendto(udpSocket,seq_num3,2000,0,(struct sockaddr *)&serverStorage,addr_size);//sending the sequence number along with that

 printf("\nPacket %d is sent\n",n3);

int nBytes3 = recvfrom(udpSocket,readbuf,45000,0,(struct sockaddr *)&serverStorage, &addr_size);//receiving the acknowledgement from the client

n = atoi(readbuf);//ascii to integer conversion for checking the acknowledgement
printf("Acknowledgement receieved %s %d\n",readbuf,n);
clock_gettime(CLOCK_REALTIME, &rtclk_stop_time);//stopping the timer to calculate RTT

if(n3 == n)//sequence number and the acknowledgement are verified for sending the next packet
{
n3++;
bytesWritten3+=m3;  
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
 bytesWritten3 -= m3;//resend the packet
r -= 1;
} 
}       

/*if the chunk contents are read, report end of the chunk*/
            if (nread < 256)
            {
                if (feof(input))
                    printf("End of file\n");
                if (ferror(input))
                    printf("Error reading\n");
                break;
            }
memset(seq_num3,0,strlen(seq_num3));//clearing the sequence number in the string
}

/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buffer_size4,0,strlen(buffer_size4));
nread =0; writebuf_ls[0] ='\0';bytesWritten3=0,bytesToWrite3=0;r=1;
fclose(input);//closing the file
    closedir(d);//closing the current directory
  }
}

/*function for exiting the process*/
else if(strcmp(pch,"exit")==0)
{
exit(0);
}

/*otherwise report invalid command*/
else
{
printf("\n Invalid command ");
}

}
  return 0;
}
