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
#include <arpa/inet.h>
#include<fcntl.h>
#include <sys/time.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>


//gcc -Wall client_md5.c -o client_md5 -lcrypto -lssl

  int i,ack=0,p=0,q=0;
  

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
  int clientSocket1,clientSocket2,clientSocket3,clientSocket4,portNum, nBytes,nBytes1,nBytes2,nBytes_ls,nBytes_user,o=0;
  char *buffer=NULL;
char *user=NULL;
  struct sockaddr_in server_info1,server_info2,server_info3,server_info4;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

/*Buffer declaration variables */
int n=0,nread,bytesReceived,bytesReceived1;
int ij,ii,cp,mod=0,digit;
char readbuf_client[45000];
char readbuf_client_ls[45000];
char writebuf_client1[45000];
char writebuf_client_1[45000];
char recvBuff11[45000]; int conn1=0;int conn2=0;int conn3=0;int conn4=0;
int end, loop;  char str[30];char *PORT1;
int f=0,gf=0,port_ser1,port_ser2,port_ser3,port_ser4;
char * ser1;
char * ser2;
char * ser3;
char * ser4;
char * ser_1;
char * ser_2;char * ser_3;char * ser_4;

    char PORT[10];
char server_1[20];char server_2[20];char server_3[20];char server_4[20];

/* Reading the configuration file */
FILE* fp11 = fopen("client_conf", "rb"); 
        if(fp11==NULL)
        {
            printf("Config file not found, so server can't start\n");
            return 1;
        }

/* ***************** Reading the PORT Number from config file*********************************** */
/* ****************** reading server1 information******************* */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
gf++;

while (gf<2)
{
ser_1 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser_1==NULL)
{
break;
}
else
{
gf++;
}
}

strcpy(server_1,ser_1);
printf("Server1 ip address is %s\n",server_1);

f++;

while (f<2)
{
ser1 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser1==NULL)
{
break;
}
else
{
f++;
}
}
ser1=strtok(ser1,"\n"); 
port_ser1 = atoi(ser1);
printf("Server1 port is %d\n",port_ser1);

end = 0; loop =0; str[0]='\0';f=0;gf=0;


/* ****************** reading server2 information******************* */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");

gf++;

while (gf<2)
{
ser_2 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser_2==NULL)
{
break;
}
else
{
gf++;
}
}

strcpy(server_2,ser_2);
printf("Server2 ip address is %s\n",server_2);

f++;

while (f<2)
{
ser2 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser2==NULL)
{
break;
}
else
{
f++;
}
}
ser2=strtok(ser2,"\n"); 
port_ser2 = atoi(ser2);
printf("Server2 port is %d\n",port_ser2);
end = 0; loop =0; str[0]='\0'; f=0;gf=0;

/* ****************** reading server3 information******************* */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");

gf++;

while (gf<2)
{
ser_3 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser_3==NULL)
{
break;
}
else
{
gf++;
}
}

strcpy(server_3,ser_3);
printf("Server3 ip address is %s\n",server_3); //server 3 ip address

f++;

while (f<2)
{
ser3 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser3==NULL)
{
break;
}
else
{
f++;
}
}
ser3=strtok(ser3,"\n"); 
port_ser3 = atoi(ser3);
printf("Server3 port is %d\n",port_ser3); //server 3 port number
end = 0; loop =0; str[0]='\0';f=0;gf=0;

/* ****************** reading server4 information******************* */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");

gf++;

while (gf<2)
{
ser_4 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser_4==NULL)
{
break;
}
else
{
gf++;
}
}

strcpy(server_4,ser_4);
printf("Server4 ip address is %s\n",server_4); //server 4 ip address

f++;

while (f<2)
{
ser4 = strtok (NULL, " ");//using string token to remove out blank spaces that might be added
if (ser4==NULL)
{
break;
}
else
{
f++;
}
}
ser4=strtok(ser4,"\n"); 
port_ser4 = atoi(ser4);
printf("Server4 port is %d\n",port_ser4);  //server 4 port number

char pass_encrypt[30];
/* ***************password encryption ******** */
end = 0; loop =0; str[0]='\0';f=0;gf=0;
/* *********reading encrytion password from config file******************** */
 for(end = loop = 0;loop<1;++loop){
        if(0==fgets(str, sizeof(str), fp11)){
            end = 1;
            break;
        }
    }
    if(!end)

PORT1 = strtok(str, " ");
strcpy(pass_encrypt,PORT1);
printf("\n passw is %s",pass_encrypt);

fclose(fp11);
/* ****************************************************************** */

/* *******************Creating sockets at the client end ******************* */

   if ((clientSocket1 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
  printf("\nsocket failure!!\n");
    }

 if ((clientSocket2 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
  printf("\nsocket failure!!\n");
    }
 if ((clientSocket3 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
  printf("\nsocket failure!!\n");
    }
 if ((clientSocket4 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
  printf("\nsocket failure!!\n");
    } 
/* *********************************************************************** */

/* *************************************servers information ***************** */
  server_info1.sin_family = AF_INET;
   server_info1.sin_port = htons((int)port_ser1);//assigning the ip address of the host
  server_info1.sin_addr.s_addr = inet_addr(server_1);//reading port number from config file
  memset(server_info1.sin_zero, '\0', sizeof server_info1.sin_zero);  //clearing the address variable
  addr_size = sizeof server_info1;//reading the address size

server_info2.sin_family = AF_INET;
  server_info2.sin_addr.s_addr = inet_addr(server_2);//reading port number from config file
   server_info2.sin_port = htons((int)port_ser2);//assigning the ip address of the host
  memset(server_info2.sin_zero, '\0', sizeof server_info2.sin_zero);  //clearing the address variable
  addr_size = sizeof server_info2;//reading the address size

server_info3.sin_family = AF_INET;
   server_info3.sin_port = htons((int)port_ser3);//reading port number from config file
  server_info3.sin_addr.s_addr = inet_addr(server_3);//assigning any port number above 5000
  memset(server_info3.sin_zero, '\0', sizeof server_info3.sin_zero);  //clearing the address variable
  addr_size = sizeof server_info3;//reading the address size

server_info4.sin_family = AF_INET;
   server_info4.sin_port = htons((int)port_ser4);//assigning the ip address of the host
  server_info4.sin_addr.s_addr = inet_addr(server_4);//reading port number from config file
  memset(server_info4.sin_zero, '\0', sizeof server_info4.sin_zero);  //clearing the address variable
  addr_size = sizeof server_info4;//reading the address size
/* ***************************************************************************************************************** */
/* ******************* connection request for tcp to servers***************************** */ 
  if (connect(clientSocket1, (struct sockaddr *)&server_info1, sizeof(struct sockaddr))<0) {
         printf("Connection Failure\n");
conn1=1;
    }

  if (connect(clientSocket2, (struct sockaddr *)&server_info2, sizeof(struct sockaddr))<0) {
         printf("Connection Failure\n");
conn2=1;
    }

  if (connect(clientSocket3, (struct sockaddr *)&server_info3, sizeof(struct sockaddr))<0) {
         printf("Connection Failure\n");
conn3=1;
    }

  if (connect(clientSocket4, (struct sockaddr *)&server_info4, sizeof(struct sockaddr))<0) {
         printf("Connection Failure\n");
conn4=1;
    } 

/* **************************************************************************** */
while(1) //while(1) characher infinite loop starting
{
int sz1=0;
int x1,x2,x3,x4;
printf("\nEnter the username and password: "); //receiving user name and password
getline(&user,&sz1,stdin);//reading the user input
nBytes_user = strlen(user) + 1; //checking the size of the buffer

/* sending the username and password to the server for verification, if the servers are not down*/
if(conn1 != 1)
{
send(clientSocket1,user, nBytes_user,0);
}

if(conn2 != 1)
{
send(clientSocket2,user, nBytes_user,0);
}
if(conn3 != 1 )
{
send(clientSocket3,user, nBytes_user,0);
}
if(conn4 != 1)
{
send(clientSocket4,user, nBytes_user,0); 
}
/********************************************************************************************* */



/*variables for file name and command parsing and storage */
char * pch;
char * pch1;
  printf("\nprintf for check1!!\n");
/*variables for count declaration of number of bytes sent or received*/
nread = 0;
 nBytes1=0;nBytes2 = 0;readbuf_client[0] = '\0';

/*command line allowing the user to enter any of the file commands*/
printf("\n get for requesting a file from the server");
printf("\n put for sending a file to the server");
printf("\n ls for reading the contents of the server directory and sending them to the client");
printf("\n list for checking wherther the file is reconstructable");
printf("\nEnter the command + file_name ");


 int sz=0;
 getline(&buffer,&sz,stdin);//reading the user input
nBytes = strlen(buffer) + 1; //checking the size of the buffer

/* sending the user command and file information to servers *************** */      
if(conn1 != 1)
{    
send(clientSocket1,buffer, nBytes,0);
}
if(conn2 != 1)
{
send(clientSocket2,buffer, nBytes,0);
}
if(conn3 != 1)
{
send(clientSocket3,buffer, nBytes,0);
}
if(conn4 != 1)
{
send(clientSocket4,buffer, nBytes,0);
}
/******************************************************************************** */

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
  printf("\nprintf for check2!!\n");
char buffer_size2[50];
char packet_num1[4];
char packet_num2[4];
char packet_num3[4];
char packet_num4[4];
char put_sub[20];
int num1,num2,num3,num4;
char seq_num[2000];
char buffer_folder[3];
int bytestoquot,bytestoquot1, bytestoquot2, bytestoquot3, bytestoquot4, bytestorem;
int n=0,n1=0,ck; int bytesToWrite;

printf("\n Enter the subfolder to write data: ");
gets(put_sub);

for(ck=0; put_sub[ck] != '\0'; ++ck);
printf("length of the folder name is %d",ck);

sprintf(buffer_folder,"%d",ck);//storing the file size in a string
puts(buffer_folder);

/* sending the subfolder name to the server to create for put ****************** */
if(conn1 != 1)
{
send(clientSocket1,put_sub,ck,0);
}

if(conn2 != 1)
{
send(clientSocket2,put_sub,ck,0);
}
if(conn3 != 1 )
{
send(clientSocket3,put_sub,ck,0);
}
if(conn4 != 1)
{
send(clientSocket4,put_sub,ck,0); 
}
/* *********************************************************************** */

x1=x2=x3=x4=0;
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added
printf("\nCommand is %s\n", pch);

/* sending the encrypted password for decrypting to servers *************** */
if(conn1 != 1)
{
send(clientSocket1,pass_encrypt,8,0);
}

if(conn2 != 1)
{
send(clientSocket2,pass_encrypt, 8,0);
}
if(conn3 != 1 )
{
send(clientSocket3,pass_encrypt, 8,0);
}
if(conn4 != 1)
{
send(clientSocket4,pass_encrypt, 8,0); 
}
/* *********************************************************************** */

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

/* ***********sending the file size to servers ************************* */
if(conn1 != 1)
{ 
send(clientSocket1,buffer_size2,5,0);
}
if(conn2 != 1)
{
send(clientSocket2,buffer_size2,5,0);
}
if(conn3 != 1)
{
send(clientSocket3,buffer_size2,5,0);
}
if(conn4 != 1)
{
send(clientSocket4,buffer_size2,5,0); 
} 

/* *********************************************************************** */
 printf("\nprintf for check3!!\n");

fclose(input);//closing the file

/* dividing the file size by 4 **************************************** */

if((bytesToWrite % 4) == 1)
{
bytestoquot = bytesToWrite / 4;
bytestoquot2 = bytestoquot; 
bytestoquot3 = bytestoquot;
bytestoquot4 = bytestoquot;
bytestoquot1 = bytestoquot + 1;
}
else if((bytesToWrite % 4) == 2)
{
bytestoquot = bytesToWrite / 4;
bytestoquot2 = bytestoquot + 1; 
bytestoquot3 = bytestoquot;
bytestoquot4 = bytestoquot;
bytestoquot1 = bytestoquot + 1;
}
else if((bytesToWrite % 4) == 3)
{
bytestoquot = bytesToWrite / 4;
bytestoquot2 = bytestoquot + 1; 
bytestoquot3 = bytestoquot + 1;
bytestoquot4 = bytestoquot;
bytestoquot1 = bytestoquot + 1;
}

if((bytesToWrite % 4) == 0)
{
bytestoquot = bytesToWrite / 4;
bytestoquot2 = bytestoquot; 
bytestoquot3 = bytestoquot;
bytestoquot4 = bytestoquot;
bytestoquot1 = bytestoquot;
}

unsigned char P1_x[bytestoquot+4];
unsigned char P2_x[bytestoquot+4];
unsigned char P3_x[bytestoquot+4];
unsigned char P4_x[bytestoquot+4];
unsigned char cj[MD5_DIGEST_LENGTH]; 

/*opening the file again in read mode*/
FILE* fp_hash = fopen(pch1,"rb");
        if(fp_hash==NULL)
        {
            printf("File open error");
            return 1;   
        }

/********************* MD5 HASH *********************************** */

MD5_CTX mdContext;
    int bytes_hash;
    unsigned char data[72400];

    MD5_Init (&mdContext);
    while ((bytes_hash = fread (data, 1, 72400, fp_hash)) != 0)
        MD5_Update (&mdContext, data, bytes_hash);
    MD5_Final (cj,&mdContext);
    for(ij = 0; ij < MD5_DIGEST_LENGTH; ij++)
{
 printf("%02x", cj[ij]);   
}
 printf (" %s\n", pch1);
for(ii=0;ii<32;ii++)
{
   digit=cj[ii]; //I presume you can convert chart to digit yourself.
   mod=(mod*16+digit) % 4;
}
printf("\n MD5HASH % 4 is %d",mod);
int m_value=mod;
fclose(fp_hash);
/* ********************************************************************** */

sprintf(packet_num1,"%d",mod);//storing the file size in a string
puts(packet_num1);

/* ***************sending the md5 % 4 value to servers ************************** */
if(conn1 != 1)
{
write(clientSocket1, packet_num1,1);
}
if(conn2 != 1)
{
write(clientSocket2, packet_num1,1);
}
if(conn3 != 1)
{
write(clientSocket3, packet_num1,1);
}
if(conn4 != 1)
{
write(clientSocket4, packet_num1,1);
}
/* ***************************************************************** */
FILE* fp = fopen(pch1,"rb");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;   
        }

int bytesWritten=0; 
int m,m1,m2,m3,m4;
/*comparing for whether the all the contents of the file to be sent to the server are read */
while(bytesWritten<bytesToWrite)
    {
if((2 * bytestoquot) > bytesToWrite-bytesWritten)
{
m = bytesToWrite - bytesWritten;
}
else
{
        m=minimum(bytestoquot,bytesToWrite-bytesWritten);
}
        unsigned char *buff = malloc(sizeof(char)*m);//creating a dynamic buffer to send every chunks of data
     
        bzero(buff, m);
printf("\n m is %d", m);
        int nread = fread(buff,1,m,fp);
        printf("\nBytes read %d \n", nread);  
       
char PX[500]; int v;int u; int s; int l;

/* ***** storing the data in four small buffers ************************ */
if(n1==0)
{
strcpy(P1_x,buff);
printf("P1_x is %s",P1_x);
  for(v=0;P1_x[v];v++)
    {
     P1_x[v]=P1_x[v]+3;
     }
printf("P1_x is %s",P1_x);
}
else if(n1==1)
{
strcpy(P2_x,buff);
printf("\n P2_x is %s",buff); 

  for(u=0;P2_x[u];u++)
    {
     P2_x[u]=P2_x[u]+3;
     }
printf("P2_x is %s",P2_x);
}
else if(n1==2)
{
strcpy(P3_x,buff); 
printf("\n P3_x is %s",buff);
  for(s=0;P3_x[s];s++)
    {
     P3_x[s]=P3_x[s]+3;
     }
printf("P3_x is %s",P3_x);
}
else if(n1==3)
{
strcpy(P4_x,buff);
printf("\n P4_x is %s",buff);
  for(l=0;P4_x[l];l++)
    {
     P4_x[l]=P4_x[l]+3;
     }
printf("P4_x is %s",P4_x);
}
/* *********************************************************************** */


/* *********************sending data to servers upon corresponding md5 % 4 value **************** */
        if(nread > 0)
        {
            printf("Sending ");

/* ***** if md5 % 4 = 0, then send (1,2), (2,3), (3,4), (1,4) ******* */
if(mod == 0)
{
if(n1==0)
{
if(conn1 != 1)
{
write(clientSocket1, P1_x, nread);
x1 = 1;
}
if(conn4 != 1)
{
write(clientSocket4, P1_x, nread);
x1=1;
}
}

if(n1==1)
{
if(conn1 != 1)
{
write(clientSocket1, P2_x, nread);
x2 = 1;
}
if(conn2 != 1)
{
write(clientSocket2, P2_x, nread);
x2=1;
}
}

if(n1==2)
{
if(conn2 != 1)
{
write(clientSocket2, P3_x, nread);
x3=1;
}
if(conn3 != 1)
{
write(clientSocket3, P3_x, nread);
x3=1;
}
}

if(n1==3)
{
if(conn3 != 1)
{
write(clientSocket3, P4_x, nread); 
x4=1;
}
if(conn4 != 1)
{
write(clientSocket4, P4_x, nread); 
x4=1;
}
}
bytesWritten+=m;
n1++;
}

/* if md5 % 4=1, then send(4,1), (1,2), (2,3), (3,4) **** */
else if(mod == 1)
{
if(n1==0)
{
if(conn1 != 1)
{
write(clientSocket1, P1_x, nread);
x1=1;
}
if(conn2 != 1)
{
write(clientSocket2, P1_x, nread);
x1=1;
}
}

if(n1==1)
{
if(conn2 != 1)
{
write(clientSocket2, P2_x, nread);
x2=1;
}
if(conn3 != 1)
{
write(clientSocket3, P2_x, nread);
x2=1;
} }
if(n1==2)
{
if(conn3 != 1)
{
write(clientSocket3, P3_x, nread);
x3=1;
}
if(conn4 != 1)
{
write(clientSocket4, P3_x, nread);
x3=1;
}
}

if(n1==3)
{
if(conn4 != 1)
{
write(clientSocket4, P4_x, nread); 
x4=1;
}
if(conn1 != 1)
{
write(clientSocket1, P4_x, nread); 
x4=1;
}
bytesWritten+=m;
n1++;
}
}

/* if md5 % 4=2, then send(3,4), (1,4), (1,2), (2,3) **** */
else if(mod == 2)
{
if(n1==0)
{
if(conn2 != 1){
write(clientSocket2, P1_x, nread);
x1=1;
}
if(conn3 != 1){
write(clientSocket3, P1_x, nread);
x1=1;
}
}
if(n1==1)
{
if(conn3 != 1){
write(clientSocket3, P2_x, nread); x2=1;}
if(conn4 != 1)
{
write(clientSocket4, P2_x, nread);
x2=1;
}
}
if(n1==2)
{
if(conn4 != 1)
{
write(clientSocket4, P3_x, nread);
x3=1;
}
if(conn1 != 1){
write(clientSocket1, P3_x, nread);x3=1;}
}
if(n1==3)
{
if(conn1 != 1){
write(clientSocket1, P4_x, nread);x4=1;}
if(conn2 != 1){
write(clientSocket2, P4_x, nread);x4=1;} 
}
bytesWritten+=m;
n1++;
}

/* if md5 % 4=3, then send(2,3), (3,4), (1,4), (1,2) **** */
else if(mod == 3)
{
if(n1==0)
{
if(conn3 != 1){
write(clientSocket3, P1_x, nread);x1=1;}
if(conn4 != 1)
{
write(clientSocket4, P1_x, nread); x1=1;
}
}
if(n1==1)
{
if(conn1 != 1){
write(clientSocket1, P2_x, nread); x2=1;}
if(conn4 != 1)
{
write(clientSocket4, P2_x, nread);x2=1;
}
}
if(n1==2)
{
if(conn1 != 1){
write(clientSocket1, P3_x, nread);x3=1;}
if(conn2 != 1){
write(clientSocket2, P3_x, nread);x3=1;}
}
if(n1==3)
{
if(conn2 != 1){
write(clientSocket2, P4_x, nread);x4=1;}
if(conn3 != 1){
write(clientSocket3, P4_x, nread);x4=1;} 
}
bytesWritten+=m;
n1++;
}

}       

/*if the chunk contents are read, report end of the chunk*/
        if (nread < m)
        {
            if (feof(fp))
                printf("End of file\n");
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
    }


/*clearing the variables for next iteration */
memset(buffer_size2,0,strlen(buffer_size2));
memset(packet_num1,0,strlen(packet_num1));
memset(put_sub,0,strlen(put_sub));
memset(writebuf_client1,0,strlen(writebuf_client1));
bytesToWrite = 0;
nread = 0;
nBytes1=0;o=1;bytesWritten = 0; 
fclose(fp);//closing the file
}


/* ***************get function to recover the splitted files ******** */ 
else if(strcmp(pch,"get")==0)
{

/*buffer initialization*/
char seq_num1[2000];int nx=0;
int bytesReceived1=0,bytesReceived2=0,bytesReceived3=0,bytesReceived4=0;
int bytesReceived1_y=0,bytesReceived2_y=0,bytesReceived3_y=0,bytesReceived4_y=0;
char recvBuff1_x[50],recvBuff2_x[50],recvBuff3_x[50],recvBuff4_x[50];
char recvBuff1_y[50],recvBuff2_y[50],recvBuff3_y[50],recvBuff4_y[50];
char get_sub[15];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added

printf("\n Enter the subfolder to read data: ");//subfolder to read data
gets(get_sub);
/* sending subfolder information to servers ***** */
if(conn1 != 1)
{
send(clientSocket1,get_sub,8,0);
}

if(conn2 != 1)
{
send(clientSocket2,get_sub,8,0);
}
if(conn3 != 1 )
{
send(clientSocket3,get_sub,8,0);
}
if(conn4 != 1)
{
send(clientSocket4,get_sub,8,0); 
}
/* ****************************************** */

/*receiving the number of bytes of the file that will be sent by the server and necessarily allocation buffer in chunks for that size */
if(conn1 != 1){
recv(clientSocket1,recvBuff1_x,5,0);}
if(conn2 != 1){
recv(clientSocket2,recvBuff2_x,5,0);}
if(conn3 != 1){
recv(clientSocket3,recvBuff3_x,5,0);}
if(conn4 != 1)
{
recv(clientSocket4,recvBuff4_x,5,0);
}
int bytesToRead1 = atoi(recvBuff1_x);//ascii to integer conversion for storing the buffer size in integer
int bytesToRead2 = atoi(recvBuff2_x);
int bytesToRead3 = atoi(recvBuff3_x);
int bytesToRead4 = atoi(recvBuff4_x);
if(conn3 == 1)
{
bytesToRead3=0;
}
  printf("Bytes read 1 %d \n",bytesToRead1);
printf("Bytes read 2 %d \n", bytesToRead2);
printf("Bytes read 3 %d \n", bytesToRead3);
printf("Bytes read 4 %d \n", bytesToRead4); 
/*opening file in write mode to store the contents sent by the server*/

int bytesToRead1_y,bytesToRead2_y,bytesToRead3_y,bytesToRead4_y;
int totalbytesread_y=0; int totalbytesread1_y=0; int totalbytesread2_y=0; int totalbytesread3_y=0; int totalbytesread4_y=0;

        int totalbytesread=0; int totalbytesread1=0; int totalbytesread2=0; int totalbytesread3=0; int totalbytesread4=0;
char recvBuff1[bytesToRead1+4];char recvBuff2[bytesToRead2+4];char recvBuff3[bytesToRead3+4];char recvBuff4[bytesToRead4+4];
  
     
char recvBuff1_z[bytesToRead1_y+4];char recvBuff2_z[bytesToRead2_y+4];char recvBuff3_z[bytesToRead3_y+4];char recvBuff4_z[bytesToRead4_y+4];
int h,l,r,q,h1,l1,r1,q1;

/* receiving second set of size of each files ************************* */
if(conn1 != 1){
recv(clientSocket1,recvBuff1_y,5,0);
bytesToRead1_y = atoi(recvBuff1_y);}
 printf("Bytes read 1 %d \n", bytesToRead1_y);
if(conn2 != 1){
recv(clientSocket2,recvBuff2_y,5,0);
bytesToRead2_y = atoi(recvBuff2_y);}
printf("Bytes read 2 %d \n", bytesToRead2_y);
if(conn3 != 1){
recv(clientSocket3,recvBuff3_y,5,0);
bytesToRead3_y = atoi(recvBuff3_y);}
printf("Bytes read 3 %d \n", bytesToRead3_y);
if(conn4 != 1)
{
recv(clientSocket4,recvBuff4_y,5,0);
bytesToRead4_y = atoi(recvBuff4_y);
}
printf("Bytes read 4 %d \n", bytesToRead4_y);

/* ***********opening a new file for storing the received data ********************* */
FILE* fp11 = fopen("client_recot", "w+"); 
        if(fp11==NULL)
        {
            printf("Error opening file");
            return 1;
        }

/* ************if md5 % 4 = 0, check for following conditions ****************** */          
if(mod == 0)      
{
if((bytesToRead4 != 0) || (bytesToRead1 != 0))
{
printf("\n Piece 1 is found ");
}
else
{
printf("\n Piece 1 is not found, file not recoverable ");
}
if((bytesToRead1_y != 0) || (bytesToRead2 != 0))
{
printf("\n Piece 2 is found ");
}
else
{
printf("\n Piece 2 is not found, file not recoverable ");
}
if((bytesToRead2_y != 0) || (bytesToRead3 != 0))
{
printf("\n Piece 3 is found ");
}
else
{
printf("\n Piece 3 is not found, file not recoverable ");
}
if((bytesToRead4_y != 0) || (bytesToRead3_y != 0))
{
printf("\n Piece 4 is found ");
}
else
{
printf("\n Piece 4 is not found, file not recoverable ");
}
printf("\n");
printf("\n");
printf("\n");

/* *******************receiving data from every file ****************************** */
if(conn1 != 1){
 bytesReceived1 = read(clientSocket1,recvBuff1,bytesToRead1);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1, recvBuff1);
for(h=0;recvBuff1[h];h++)
     {
     recvBuff1[h]=recvBuff1[h]-3;
     }
if(bytesReceived1 == bytesToRead1)
{
fwrite(recvBuff1, 1,bytesToRead1,fp11);
}
}
else
{ 
if(conn4 != 1)
{
bytesReceived4 = read(clientSocket4,recvBuff4,bytesToRead4);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4, recvBuff4);

for(l=0;recvBuff4[l];l++)
     {
     recvBuff4[l]=recvBuff4[l]-3;
     }
fwrite(recvBuff4, 1,bytesToRead4,fp11);
}
}

if(conn2 != 1){
 bytesReceived2 = read(clientSocket2,recvBuff2,bytesToRead2);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2, recvBuff2);
for(r=0;recvBuff2[r];r++)
     {
     recvBuff2[r]=recvBuff2[r]-3;
     } 
if(bytesReceived2 == bytesToRead2)
{
fwrite(recvBuff2, 1,bytesToRead2,fp11);
}}
else
{
if(conn1 != 1){
 bytesReceived1_y = read(clientSocket1,recvBuff1_z,bytesToRead1_y);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1_y, recvBuff1_z);
for(q=0;recvBuff1_z[q];q++)
     {
     recvBuff1_z[q]=recvBuff1_z[q]-3;
     }
fwrite(recvBuff1_z, 1,bytesToRead1_y,fp11);}
}

if(conn3 != 1){
 bytesReceived3 = read(clientSocket3,recvBuff3,bytesToRead3);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3, recvBuff3);
for(h1=0;recvBuff3[h1];h1++)
     {
     recvBuff3[h1]=recvBuff3[h1]-3;
     }
if(bytesReceived3==bytesToRead3)
{
fwrite(recvBuff3, 1,bytesToRead3,fp11);
}}
else
{ 
if(conn2 != 1){
bytesReceived2_y = read(clientSocket2,recvBuff2_z,bytesToRead2_y);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2_y, recvBuff2_z);
for(l1=0;recvBuff2_z[l1];l1++)
     {
     recvBuff2_z[l1]=recvBuff2_z[l1]-3;
     }
fwrite(recvBuff2_z, 1,bytesToRead2_y,fp11);}
}

if(conn3 != 1){
 bytesReceived3_y = read(clientSocket3,recvBuff3_z,bytesToRead3_y);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3_y, recvBuff3_z);
for(r1=0;recvBuff3_z[r1];r1++)
     {
     recvBuff3_z[r1]=recvBuff3_z[r1]-3;
     }
if(bytesReceived3_y==bytesToRead3_y)
{
fwrite(recvBuff3_z, 1,bytesToRead3_y,fp11);
}}
else
{
if(conn4 != 1)
{
bytesReceived4_y = read(clientSocket4,recvBuff4_z,bytesToRead4_y);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4_y, recvBuff4_z);
for(q1=0;recvBuff4_z[q1];q1++)
     {
     recvBuff4_z[q1]=recvBuff4_z[q1]-3;
     }
fwrite(recvBuff4_z, 1,bytesToRead4_y,fp11);
}
}

}

/* ************if md5 % 4 = 1, check for following conditions ****************** */          
else if(mod == 1)      
{
if((bytesToRead2 != 0) || (bytesToRead1 != 0))
{
printf("\n Piece 1 is found ");
}
else
{
printf("\n Piece 1 is not found, file not recoverable ");
}
if((bytesToRead2_y != 0) || (bytesToRead3 != 0))
{
printf("\n Piece 2 is found ");
}
else
{
printf("\n Piece 2 is not found, file not recoverable ");
}
if((bytesToRead3_y != 0) || (bytesToRead4 != 0))
{
printf("\n Piece 3 is found ");
}
else
{
printf("\n Piece 3 is not found, file not recoverable ");
}
if((bytesToRead4_y != 0) || (bytesToRead1_y != 0))
{
printf("\n Piece 4 is found ");
}
else
{
printf("\n Piece 4 is not found, file not recoverable ");
}
printf("\n");
printf("\n");
printf("\n");

/* *******************receiving data from every file ****************************** */
if(conn1 != 1){
 bytesReceived1 = read(clientSocket1,recvBuff1,bytesToRead1);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1, recvBuff1);
for(h=0;recvBuff1[h];h++)
     {
     recvBuff1[h]=recvBuff1[h]-3;
     }}
if(bytesReceived1 == bytesToRead1)
{
fwrite(recvBuff1, 1,bytesToRead1,fp11);
}
else
{ 
if(conn2 != 1)
{
bytesReceived2 = read(clientSocket2,recvBuff2,bytesToRead2);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2, recvBuff2);

for(l=0;recvBuff2[l];l++)
     {
     recvBuff2[l]=recvBuff2[l]-3;
     }
fwrite(recvBuff2, 1,bytesToRead2,fp11);
}
}

if(conn3 != 1){
 bytesReceived3 = read(clientSocket3,recvBuff3,bytesToRead3);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3, recvBuff3);
for(r=0;recvBuff3[r];r++)
     {
     recvBuff3[r]=recvBuff3[r]-3;
     } 
if(bytesReceived3 == bytesToRead3)
{
fwrite(recvBuff3, 1,bytesToRead3,fp11);
}}
else
{
if(conn2 != 1){
 bytesReceived2_y = read(clientSocket2,recvBuff2_z,bytesToRead2_y);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2_y, recvBuff2_z);
for(q=0;recvBuff2_z[q];q++)
     {
     recvBuff2_z[q]=recvBuff2_z[q]-3;
     }
fwrite(recvBuff2_z, 1,bytesToRead2_y,fp11);}
}

if(conn4 != 1){
 bytesReceived4 = read(clientSocket4,recvBuff4,bytesToRead4);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4, recvBuff4);
for(h1=0;recvBuff4[h1];h1++)
     {
     recvBuff4[h1]=recvBuff4[h1]-3;
     }
if(bytesReceived4==bytesToRead4)
{
fwrite(recvBuff4, 1,bytesToRead4,fp11);
}}
else
{ 
if(conn3 != 1){
bytesReceived3_y = read(clientSocket3,recvBuff3_z,bytesToRead3_y);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3_y, recvBuff3_z);
for(l1=0;recvBuff3_z[l1];l1++)
     {
     recvBuff3_z[l1]=recvBuff3_z[l1]-3;
     }
fwrite(recvBuff3_z, 1,bytesToRead3_y,fp11);}
}

if(conn1 != 1){
 bytesReceived1_y = read(clientSocket1,recvBuff1_z,bytesToRead1_y);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1_y, recvBuff1_z);
for(r1=0;recvBuff1_z[r1];r1++)
     {
     recvBuff1_z[r1]=recvBuff1_z[r1]-3;
     }
if(bytesReceived1_y==bytesToRead1_y)
{
fwrite(recvBuff1_z, 1,bytesToRead1_y,fp11);
}}
else
{
if(conn4 != 1)
{
bytesReceived4_y = read(clientSocket4,recvBuff4_z,bytesToRead4_y);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4_y, recvBuff4_z);
for(q1=0;recvBuff4_z[q1];q1++)
     {
     recvBuff4_z[q1]=recvBuff4_z[q1]-3;
     }
fwrite(recvBuff4_z, 1,bytesToRead4_y,fp11);
}
}

}

/* ************if md5 % 4 = 2, check for following conditions ****************** */          
else if(mod == 2)      
{
if((bytesToRead3 != 0) || (bytesToRead3 != 0))
{
printf("\n Piece 1 is found ");
}
else
{
printf("\n Piece 1 is not found, file not recoverable ");
}
if((bytesToRead3_y != 0) || (bytesToRead4 != 0))
{
printf("\n Piece 2 is found ");
}
else
{
printf("\n Piece 2 is not found, file not recoverable ");
}
if((bytesToRead4_y != 0) || (bytesToRead1 != 0))
{
printf("\n Piece 3 is found ");
}
else
{
printf("\n Piece 3 is not found, file not recoverable ");
}
if((bytesToRead1_y != 0) || (bytesToRead2_y != 0))
{
printf("\n Piece 4 is found ");
}
else
{
printf("\n Piece 4 is not found, file not recoverable ");
}
printf("\n");
printf("\n");
printf("\n");

/* *******************receiving data from every file ****************************** */
if(conn3 != 1){
 bytesReceived3 = read(clientSocket3,recvBuff3,bytesToRead3);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3, recvBuff3);
for(h=0;recvBuff3[h];h++)
     {
     recvBuff3[h]=recvBuff3[h]-3;
     }}
if(bytesReceived3 == bytesToRead3)
{
fwrite(recvBuff3, 1,bytesToRead3,fp11);
}
else
{ 
if(conn2 != 1)
{
bytesReceived2 = read(clientSocket2,recvBuff2,bytesToRead2);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2, recvBuff2);

for(l=0;recvBuff2[l];l++)
     {
     recvBuff2[l]=recvBuff2[l]-3;
     }
fwrite(recvBuff2, 1,bytesToRead2,fp11);
}
}

if(conn4 != 1){
 bytesReceived4 = read(clientSocket4,recvBuff4,bytesToRead4);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4, recvBuff4);
for(r=0;recvBuff4[r];r++)
     {
     recvBuff4[r]=recvBuff4[r]-3;
     } 
if(bytesReceived4 == bytesToRead4)
{
fwrite(recvBuff4, 1,bytesToRead4,fp11);
}}
else
{
if(conn3 != 1){
 bytesReceived3_y = read(clientSocket3,recvBuff3_z,bytesToRead3_y);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3_y, recvBuff3_z);
for(q=0;recvBuff3_z[q];q++)
     {
     recvBuff3_z[q]=recvBuff3_z[q]-3;
     }
fwrite(recvBuff3_z, 1,bytesToRead3_y,fp11);}
}

if(conn1 != 1){
 bytesReceived1 = read(clientSocket1,recvBuff1,bytesToRead1);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1, recvBuff1);
for(h1=0;recvBuff1[h1];h1++)
     {
     recvBuff1[h1]=recvBuff1[h1]-3;
     }
if(bytesReceived1==bytesToRead1)
{
fwrite(recvBuff1, 1,bytesToRead1,fp11);
}}
else
{ 
if(conn4 != 1){
bytesReceived4_y = read(clientSocket4,recvBuff4_z,bytesToRead4_y);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4_y, recvBuff4_z);
for(l1=0;recvBuff4_z[l1];l1++)
     {
     recvBuff4_z[l1]=recvBuff4_z[l1]-3;
     }
fwrite(recvBuff4_z, 1,bytesToRead4_y,fp11);}
}

if(conn1 != 1){
 bytesReceived1_y = read(clientSocket1,recvBuff1_z,bytesToRead1_y);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1_y, recvBuff1_z);
for(r1=0;recvBuff1_z[r1];r1++)
     {
     recvBuff1_z[r1]=recvBuff1_z[r1]-3;
     }
if(bytesReceived1_y==bytesToRead1_y)
{
fwrite(recvBuff1_z, 1,bytesToRead1_y,fp11);
}}
else
{
if(conn2 != 1)
{
bytesReceived2_y = read(clientSocket2,recvBuff2_z,bytesToRead2_y);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2_y, recvBuff2_z);
for(q1=0;recvBuff2_z[q1];q1++)
     {
     recvBuff2_z[q1]=recvBuff2_z[q1]-3;
     }
fwrite(recvBuff2_z, 1,bytesToRead2_y,fp11);
}
}

}

/* ************if md5 % 4 = 0, check for following conditions ****************** */          
else if(mod == 3)      
{
if((bytesToRead4 != 0) || (bytesToRead3 != 0))
{
printf("\n Piece 1 is found ");
}
else
{
printf("\n Piece 1 is not found, file not recoverable ");
}
if((bytesToRead4_y != 0) || (bytesToRead1 != 0))
{
printf("\n Piece 2 is found ");
}
else
{
printf("\n Piece 2 is not found, file not recoverable ");
}
if((bytesToRead1_y != 0) || (bytesToRead2 != 0))
{
printf("\n Piece 3 is found ");
}
else
{
printf("\n Piece 3 is not found, file not recoverable ");
}
if((bytesToRead2_y != 0) || (bytesToRead3_y != 0))
{
printf("\n Piece 4 is found ");
}
else
{
printf("\n Piece 4 is not found, file not recoverable ");
}
printf("\n");
printf("\n");
printf("\n");

/* *******************receiving data from every file ****************************** */
if(conn3 != 1){
 bytesReceived3 = read(clientSocket3,recvBuff3,bytesToRead3);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3, recvBuff3);
for(h=0;recvBuff3[h];h++)
     {
     recvBuff3[h]=recvBuff3[h]-3;
     }}
if(bytesReceived3 == bytesToRead3)
{
fwrite(recvBuff3, 1,bytesToRead3,fp11);
}
else
{ 
if(conn4 != 1)
{
bytesReceived4 = read(clientSocket4,recvBuff4,bytesToRead4);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4, recvBuff4);

for(l=0;recvBuff4[l];l++)
     {
     recvBuff4[l]=recvBuff4[l]-3;
     }
fwrite(recvBuff4, 1,bytesToRead4,fp11);
}
}

if(conn1 != 1){
 bytesReceived1 = read(clientSocket1,recvBuff1,bytesToRead1);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1, recvBuff1);
for(r=0;recvBuff1[r];r++)
     {
     recvBuff1[r]=recvBuff1[r]-3;
     } 
if(bytesReceived1 == bytesToRead1)
{
fwrite(recvBuff1, 1,bytesToRead1,fp11);
}}
else
{
if(conn4 != 1){
 bytesReceived4_y = read(clientSocket4,recvBuff4_z,bytesToRead4_y);//reading the packets of data from the server
            printf("Bytes received 4 %d %s\n",bytesReceived4_y, recvBuff4_z);
for(q=0;recvBuff4_z[q];q++)
     {
     recvBuff4_z[q]=recvBuff4_z[q]-3;
     }
fwrite(recvBuff4_z, 1,bytesToRead4_y,fp11);}
}

if(conn2 != 1){
 bytesReceived2 = read(clientSocket2,recvBuff2,bytesToRead2);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2, recvBuff2);
for(h1=0;recvBuff2[h1];h1++)
     {
     recvBuff2[h1]=recvBuff2[h1]-3;
     }
if(bytesReceived2==bytesToRead2)
{
fwrite(recvBuff2, 1,bytesToRead2,fp11);
}}
else
{ 
if(conn1 != 1){
bytesReceived1_y = read(clientSocket1,recvBuff1_z,bytesToRead1_y);//reading the packets of data from the server
            printf("Bytes received 1 %d %s\n",bytesReceived1_y, recvBuff1_z);
for(l1=0;recvBuff1_z[l1];l1++)
     {
     recvBuff1_z[l1]=recvBuff1_z[l1]-3;
     }
fwrite(recvBuff1_z, 1,bytesToRead1_y,fp11);}
}

if(conn3 != 1){
 bytesReceived3_y = read(clientSocket3,recvBuff3_z,bytesToRead3_y);//reading the packets of data from the server
            printf("Bytes received 3 %d %s\n",bytesReceived3_y, recvBuff3_z);
for(r1=0;recvBuff3_z[r1];r1++)
     {
     recvBuff3_z[r1]=recvBuff3_z[r1]-3;
     }
if(bytesReceived3_y==bytesToRead3_y)
{
fwrite(recvBuff3_z, 1,bytesToRead3_y,fp11);
}}
else
{
if(conn2 != 1)
{
bytesReceived2_y = read(clientSocket2,recvBuff2_z,bytesToRead2_y);//reading the packets of data from the server
            printf("Bytes received 2 %d %s\n",bytesReceived2_y, recvBuff2_z);
for(q1=0;recvBuff2_z[q1];q1++)
     {
     recvBuff2_z[q1]=recvBuff2_z[q1]-3;
     }
fwrite(recvBuff2_z, 1,bytesToRead2_y,fp11);
}
}

}
fclose(fp11); 
/****************** closing the file after writing data ********************* */

/*clearing the variables for next iteration */
p = 0;nx=0;
memset(recvBuff1_x,0,strlen(recvBuff1_x));
memset(recvBuff2_x,0,strlen(recvBuff2_x));
memset(recvBuff3_x,0,strlen(recvBuff3_x));
memset(recvBuff4_x,0,strlen(recvBuff4_x));
memset(recvBuff1_y,0,strlen(recvBuff1_y));
memset(recvBuff2_y,0,strlen(recvBuff2_y));
memset(recvBuff3_y,0,strlen(recvBuff3_y));
memset(recvBuff4_y,0,strlen(recvBuff4_y));
memset(recvBuff1_z,0,strlen(recvBuff1_z));
memset(recvBuff2_z,0,strlen(recvBuff2_z));
memset(recvBuff3_z,0,strlen(recvBuff3_z));
memset(recvBuff4_z,0,strlen(recvBuff4_z));
nread = 0; writebuf_client1[0] = '\0'; nBytes1=0; bytesReceived1=0;
bytesReceived2=0;bytesReceived3=0;bytesReceived4=0;totalbytesread1=0;bytesToRead1=0;bytesToRead2=0;
bytesToRead3=0;bytesToRead4=0;
bytesReceived1_y=0;
bytesReceived2_y=0;bytesReceived3_y=0;bytesReceived4_y=0;totalbytesread1_y=0;bytesToRead1_y=0;bytesToRead2_y=0;
bytesToRead3_y=0;bytesToRead4_y=0;

}

/* function for displaying just whether the file pieces are reconstructable ******************** */
else if(strcmp(pch,"list")==0)
{
if(x1==1)
{
printf("\n Piece 1 is found ");
}
else
{
printf("\n Piece 1 is not found, file not recoverable ");
} 
if(x2==1)
{
printf("\n Piece 2 is found ");
}
else
{
printf("\n Piece 2 is not found, file not recoverable ");
} 
if(x3==1)
{
printf("\n Piece 3 is found ");
}
else
{
printf("\n Piece 3 is not found, file not recoverable ");
} 
if(x4==1)
{
printf("\n Piece 4 is found ");
}
else
{
printf("\n Piece 4 is not found, file not recoverable ");
} 
}
/* ****************************************************************************** */


/* function for receiving the folder directory contents of servers ******************* */
else if(strcmp(pch,"ls")==0)
{
/*buffer initialization*/
char recvBuff2_y[50];char seq_num2[2000];char recvBuff2_x[50];char recvBuff2_z[50];char recvBuff2_yz[50];
int bytesReceived1_x,bytesReceived2_x,bytesReceived3_x,bytesReceived4_x;
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n");//using string token to remove out new line characters that might be added

/*receiving the number of bytes of the file to be received from the server and necessarily allocation buffer in chunks for that size */
if(conn1 != 1){
recv(clientSocket1,recvBuff2_x,3,0);}
if(conn2 != 1){
recv(clientSocket2,recvBuff2_y,3,0);}
if(conn3 != 1){
recv(clientSocket3,recvBuff2_z,3,0);}
if(conn4 != 1)
{
recv(clientSocket4,recvBuff2_yz,3,0);
}
int bytesToRead1 = atoi(recvBuff2_x);//ascii to integer conversion for storing the buffer size in integer
int bytesToRead2 = atoi(recvBuff2_y);
int bytesToRead3 = atoi(recvBuff2_z);
int bytesToRead4 = atoi(recvBuff2_yz);


   printf("Bytes read 1 %d \n", bytesToRead1);
 printf("Bytes read 2 %d \n", bytesToRead2);
 printf("Bytes read 3 %d \n", bytesToRead3);
printf("Bytes read 4 %d \n", bytesToRead4);
     
        int totalbytesread1=0;

/*comparing for whether the all the contents of the file sent by the server are read */

      while(totalbytesread1 < bytesToRead1+bytesToRead2+bytesToRead3+bytesToRead4)
     //while(totalbytesread1 < bytesToRead1+bytesToRead2+bytesToRead3+by)        
{
            int m1 = minimum(256,bytesToRead1-totalbytesread1);
 int m2 = minimum(256,bytesToRead2-totalbytesread1);
 int m3 = minimum(256,bytesToRead3-totalbytesread1);
 int m4 = minimum(256,bytesToRead4-totalbytesread1);
            char *recvBuff1=malloc(sizeof(char)*m1);//creating a dynamic buffer to store every chunks of data
            bzero(recvBuff1,m1);
 char *recvBuff2=malloc(sizeof(char)*m2);
 bzero(recvBuff2,m2);
char *recvBuff3=malloc(sizeof(char)*m3);
 bzero(recvBuff3,m3);
char *recvBuff4=malloc(sizeof(char)*m4);
 bzero(recvBuff4,m4);
if(conn1 != 1){ 
 bytesReceived1_x = recv(clientSocket1,recvBuff1,m1,0);//reading the packets of data from the server
 }        

            printf(" DFS1 Bytes received %d \n %s\n",bytesReceived1_x,recvBuff1);
if(conn2 != 1){
 bytesReceived2_x = recv(clientSocket2,recvBuff2,m2,0);//reading the packets of data from the server
         }

            printf("DFS2 Bytes received %d \n %s\n",bytesReceived2_x,recvBuff2);
if(conn3 != 1){
 bytesReceived3_x = recv(clientSocket3,recvBuff3,m3,0);//reading the packets of data from the server
         }
            printf("DFS3 Bytes received %d \n %s\n",bytesReceived3_x,recvBuff3);

if(conn4 != 1)
{
 bytesReceived4_x = recv(clientSocket4,recvBuff4,m4,0);//reading the packets of data from the server
}         
            printf("DFS4 Bytes received %d \n %s\n",bytesReceived4_x,recvBuff4);


           totalbytesread1 += bytesReceived1_x+bytesReceived2_x+bytesReceived3_x+bytesReceived4_x; //add the bytes received to store in a file  


}

  
/*clearing the variables for next iteration */
memset(writebuf_client1,0,strlen(writebuf_client1));
memset(recvBuff2_x,0,strlen(recvBuff2_x));
memset(recvBuff2_y,0,strlen(recvBuff2_y));
memset(recvBuff2_z,0,strlen(recvBuff2_z));
memset(recvBuff2_yz,0,strlen(recvBuff2_yz));
nread = 0; writebuf_client1[0] = '\0'; nBytes1=0; bytesReceived1_x=bytesReceived2_x=bytesReceived3_x=bytesReceived4_x=0;
bytesToRead1=bytesToRead2=bytesToRead3=0;bytesToRead4=0;
//fclose(fp11);//closing the file
  }


}//while(1) ending
return 0;
}
 
