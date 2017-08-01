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

#include <sched.h> /* real time schedulers*/ 

#include <arpa/inet.h>




#define BACKLOG 10
pthread_mutex_t msgSem1,msgSem2,msgSem3,msgSem4;

int i,o=0,p=0,r=0;


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
void *server1(void *args)
{

 int clientSocket1, nBytes,nBytes3,o=0;
  int yes =1;
/*Buffer declaration variables */
  char buffer[65000];
char buffer_user[6000];
char readbuf[65000];
char writebuf[65000];
char writebuf_server[65000];
char writebuf_server2[65000];
char writebuf_ls[65000];

/*Variables for client server socket addressing */ 
  struct sockaddr_in serverAddr, clientAddr;
 struct sockaddr_in dest;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
 int status,socket_fd, client_fd,num,client_ad;
    socklen_t size;

/*variables for count declaration of number of bytes sent or received*/
  int i,n=0,bytesReceived,bytesReceived2,bytesReceived_x,nuser=0,nuser_gf=0; 

/*variables for file name and command parsing and storage */
char * pch;
char * pch_user;
char * pch1;
char * pch1_user;
char * ser_nuser;
char server_nuser[200];char pch1_nuser[200];
//int needed;

//char * store1;
//char * store2;

/*Variables for opening and reading the contents of the server directory*/
DIR *d;
struct dirent *dir;

int nread;

   if ((clientSocket1 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
      printf("\nSocket failure!!\n");
        fprintf(stderr, "Socket failure!!\n");
    //    exit(1);
    }

 memset(&serverAddr, 0, sizeof(serverAddr));
memset(&dest,0,sizeof(dest));
  serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons((int)10001); //assigning the ip address of the client
  serverAddr.sin_addr.s_addr = INADDR_ANY;//assigning any port number in accordance to the client


   if ((bind(clientSocket1, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))== -1)    { 
  printf("\nBind failure!!\n");      
  fprintf(stderr, "Binding Failure\n");
   //     exit(1);
    }

/* **************listening for connections *********************** */
 if ((listen(clientSocket1, 10))== -1){
       printf("\nlisten failure!!\n");
        fprintf(stderr, "Listening Failure\n");
   //     exit(1);
    }
/******************************************************************/
  printf("\nprintf for check1!!\n");
/*zeroing the contents of buffers before alocation */
bzero(buffer,sizeof(buffer));   
bzero(buffer_user,sizeof(buffer_user)); 
bzero(buffer,sizeof(readbuf));
while(1)
{
  size = sizeof(struct sockaddr_in);
client_ad = sizeof(dest);
  printf("\nprintf for check2!!\n");
/* ************************** accepting connections ************************** */
 client_fd = accept(clientSocket1, (struct sockaddr*)&dest ,&client_ad);
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
/* ***************************************************************************** */


 if (client_fd<0)              //if there are negative request, print error
            error ("accept() error");
        else                        //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
                

while(1)  //while(1) characher infinite loop starting
{

/* **********************login ********************************* */
recv(client_fd,buffer_user,sizeof(buffer_user),0);
printf("\n the user is %s",buffer_user); 

pch_user = strtok (buffer_user," "); //using string token to separate command and filename

strcpy(server_nuser,pch_user);
printf("User name pch is %s\n",server_nuser);

nuser++;
while (nuser<2)
{
pch1_user = strtok (NULL, " ");//using string token again to remove out new line characters
if (pch1_user==NULL)
{
break;
}
else
{
nuser++;
}
}
pch1_user=strtok(pch1_user,"\n"); 
printf("\npassword is %s",pch1_user);
strcpy(pch1_nuser,pch1_user);
printf("\npch1_nuser is %s",pch1_nuser);

FILE *pf=fopen("server_conf","r");
    char tmp[256]={0x0};
    while(pf!=NULL && fgets(tmp, sizeof(tmp), pf)!=NULL)
    {
        if (strstr(tmp,server_nuser))
{
         
               if (strstr(tmp,pch1_nuser))
{ 
           printf("\n%s", tmp);
printf("\n valid");
goto LOOP;
} 
else
{
printf("\n wrong password");
}
}
else
{
printf("\n User invalid");
}
   }
    if(pf!=NULL) fclose(pf);

exit(0);

LOOP:
nuser =0; //server_nuser[0] = '\0';
 pch1_nuser[0]='\0';buffer_user[0]='\0';
  memset(pch1_user,0,strlen(pch1_user));
 memset(buffer_user,0,strlen(buffer_user));
DIR *d;
  struct dirent *dir;

if (strcmp(server_nuser,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Alice";
chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(server_nuser,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Bob";
chdir (directory); 
printf("\n We are under Bob folder ");
}

char user_server[30];
strcpy(user_server,server_nuser);
 memset(server_nuser,0,strlen(server_nuser));
/* *********************************************************************** */

printf("\nprintf for check3!!\n");
nread =0; writebuf[0] ='\0';nBytes3 = 0;readbuf[0] = '\0';//clearing the buffers and variables for next iteration

recv(client_fd,buffer,sizeof(buffer),0);

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
char filename[30];
strcpy(filename,pch1);

char STR1[30]; char STR2[30]; 
char PTR1[30]; char PTR2[30]; 
int str_1,str_2;
char folder[250];

/*function for put-client to server */
if(strcmp(pch,"put")==0)
{
  printf("\nprintf for check4!!\n");

/*buffer allocation */
char buffer_size1[50]; char packet_num1[4];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added

int status;

DIR* dir = opendir(pch1);
if(!dir)
{
status = mkdir(pch1, 0777);
}

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice Put folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob Put folder ");
}
/*receiving the number of bytes of the file to be sent and necessarily allocation buffer in chunks for that size */
recv(client_fd,buffer_size1,5,0);


puts(buffer_size1);
int bytesToRead = atoi(buffer_size1); //ascii to integer conversion for storing the buffer size in integer
int br=strlen(buffer_size1);
   printf("Bytes read  %d %d\n", bytesToRead,br);
int bytesby4 = bytesToRead / 4;
/*opening file in write mode to store the contents sent by the client*/
    
int num_x,num_y;
num_x = read(client_fd, packet_num1,1);
            printf("\npacket num1 %d %s\n",num_x, packet_num1);

int mod = atoi(packet_num1);
int needed = strlen(pch1) + 2;

char * store1 = (char *) malloc( sizeof( char ) * needed );
char * store2 = (char *) malloc( sizeof( char ) * needed );

if (mod == 0)
{
strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".2");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 1)
{
strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 2)
{
strcpy( store1, pch1);
strcat(store1,".3");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 3)
{

strcpy( store1, pch1);
strcat(store1,".2");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".3");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}

strcpy(PTR1,STR1);
strcpy(PTR2,STR2);
FILE *fp;
        fp = fopen(store1, "wb"); 
        if(NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
       
        int totalbytesread=0;

char recvBuff[bytesby4+1];char recvBuff_x[bytesby4+1]; //char packet_num1[4]; //char packet_num2[4];
int h,l;
bytesReceived = read(client_fd, recvBuff,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived,recvBuff); 
str_1=bytesReceived;
 for(h=0;recvBuff[h];h++)
     {
     recvBuff[h]=recvBuff[h]-3;
     }

fwrite(recvBuff, 1,bytesReceived,fp);
bytesReceived_x = read(client_fd, recvBuff_x,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived_x,recvBuff_x);
str_2=bytesReceived_x;
 for(l=0;recvBuff_x[l];l++)
     {
     recvBuff_x[l]=recvBuff_x[l]-3;
     }

        if(bytesReceived < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }
 if(bytesReceived_x < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
o = 0;
memset(buffer_size1,0,strlen(buffer_size1));
memset(recvBuff,0,strlen(recvBuff));
memset(packet_num1,0,strlen(packet_num1));
nread = 0; writebuf_server2[0] = '\0';bytesReceived=0;//bytesReceived_x=0;//bytesToRead=0;
totalbytesread=0;
num_x=0;num_y=0;
fclose(fp);//closing the file
//pch1[0]++;
FILE *p3;
p3 = fopen(store2,"wb");
  if(NULL == p3)
        {
            printf("Error opening file");
            return 1;
        }
  fwrite(recvBuff_x, 1,bytesReceived_x,p3);//write the data in a file to be stored at server local directory
fclose(p3);
memset(recvBuff_x,0,strlen(recvBuff_x));
bytesToRead=0;bytesReceived_x=0;bytesby4=0;
char *directory1 = "/home/swaminath/Downloads/split/pairs/server_thread/server1";
chdir (directory1); 
}

else if(strcmp(pch,"get")==0)
{

char buffer_size3[4000];char buffer_size[4000];
int bytesToWrite2=0,n=0,n2=0;int bytesToWrite3=0;
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}

/*opening file in read mode to send the contents of server to the client*/

FILE *input2 = fopen(STR1,"rb");
//FILE *input2 = fopen("client_file.txt.1","rb");
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

int nobytes = send(client_fd,buffer_size,5,0);
printf("\n nobytes is %d",nobytes); 
fclose(input2);//closing the file

FILE *input3 = fopen(STR2,"rb");
//FILE *input3 = fopen("client_file.txt.2","rb");  
      if(input3==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(input3, 0, SEEK_END);
bytesToWrite3 = ftell(input3);//reading the size of the file 
printf("bytes to write is %d",bytesToWrite3);

int bytes3=(int)bytesToWrite3;
sprintf(buffer_size3,"%d",bytes3);//writing the size of the file in bytes to the client
puts(buffer_size3);
send(client_fd,buffer_size3,5,0);
fclose(input3);//closing the file


/*opening the file in read mode*/
FILE* fp2 = fopen(STR1,"rb");
//FILE* fp2 = fopen("client_file.txt.1","rb");  
      if(fp2==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten2=0; int r,q;

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get2[bytesToWrite2+4];
 
int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);        
printf("\nBytes read %d \n", nread2);  
 for(r=0;buff_get2[r];r++)
     {
     buff_get2[r]=buff_get2[r]+3;
     }

         
 int bytestowr2 = write(client_fd,buff_get2,nread2);
 printf("Bytes sent 3 %d %s\n",bytestowr2, buff_get2);
 
  
    
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get2,0,strlen(buff_get2));
memset(buffer_size,0,strlen(buffer_size));
nread2 = 0;
 bytes=0;
readbuf[0] = '\0';p=1;//bytesWritten2 = 0;n2=0;n=0;RTT=0;
bytesToWrite2 = 0;bytesWritten2=0;//m2=0;
bytestowr2=0;
fclose(fp2);//closing the file

//pch1[0]++;



/*opening the file in read mode*/
FILE* fp3 = fopen(STR2,"rb");
//FILE* fp3 = fopen("client_file.txt.2","rb");  
      if(fp3==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten3=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get3[bytesToWrite3+4];

//        int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);//reading the number of bytes sent
int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);        
printf("\nBytes read %d \n", nread3);  

  for(q=0;buff_get3[q];q++)
     {
     buff_get3[q]=buff_get3[q]+3;
     }
             
   int byteswr3 = write(client_fd,buff_get3,nread3);
 printf("Bytes sent 3 %d %s\n",byteswr3, buff_get3);

/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get3,0,strlen(buff_get3));
memset(buffer_size3,0,strlen(buffer_size3));
memset(STR1,0,strlen(STR1));
memset(STR2,0,strlen(STR2));
nread3=0; 
readbuf[0] = '\0';p=1;bytes3=0;
bytesToWrite2 = 0;bytesToWrite3 = 0;bytesWritten3=0;
fclose(fp3);//closing the file
char *directory2 = "/home/swaminath/Downloads/split/pairs/server_thread/server1";
chdir (directory2); 
}

else if (strcmp(pch,"ls")==0)
{
printf("\nfile name is %s",pch1);
printf("\n filename is %s",filename);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);
int status1;
if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n LS folder is %s",folder);
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server1/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}
/*directory variable initialization*/
  DIR *d;
  struct dirent *dir;
/*variables for reading the buffer and data packets */
int bytesToWrite3=0,n3=0;
char storage[256]; char seq_num3[2000];
char buffer_size4[50];
char buf_PTR1[str_1+1];
char buf_PTR2[str_2+1];
int bytesToWritex=0;int bytesToWritex1=0;
FILE *inputx = fopen(PTR1,"rb");
        if(inputx==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputx, 0, SEEK_END);
bytesToWritex = ftell(inputx);//reading the size of the file 
printf("bytes to write is %d",bytesToWritex);
fclose(inputx);

FILE *inputy = fopen(PTR2,"rb");
        if(inputy==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputy, 0, SEEK_END);
bytesToWritex1 = ftell(inputy);//reading the size of the file 
printf("bytes to write x1 is %d",bytesToWritex1);
fclose(inputy);

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

if(bytesToWritex1 != str_2)
{
if(strcmp(dir->d_name,PTR2)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 

if(bytesToWritex != str_1)
{
if(strcmp(dir->d_name,PTR1)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 
 
//printf("%s [Incomplete]\n", dir->d_name);

else
{
     printf("%s\n", dir->d_name);
fprintf(fp4, "%s\n",dir->d_name);
}

 //file write the contents
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

send(client_fd,buffer_size4,3,0);
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
    
 if(nread > 0)
 {
            printf("Sending: %s\n",buff_ls);
send(client_fd,buff_ls,nread,0);
bytesWritten3+=m3;  
}
 
}       
          
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buffer_size4,0,strlen(buffer_size4));
nread =0; writebuf_ls[0] ='\0';bytesWritten3=0,bytesToWrite3=0;r=1;
fclose(input);//closing the file
    closedir(d);//closing the current directory
  }
char *directory3 = "/home/swaminath/Downloads/split/pairs/server_thread/server1";
chdir (directory3); 
}


 //closedir(d);//closing the current directory
server_nuser[0] = '\0';
 memset(server_nuser,0,strlen(server_nuser));
}

}
}

  //Close Connection Socket
        close(client_fd);
    } //Outer While
  sleep(10);
}

/*Main function with two arguments-ip address and the port number */
void *server2(void *args){
sleep(10);
 pthread_mutex_lock(&msgSem2);
 int clientSocket2, nBytes,nBytes3,o=0;
  int yes =1; int client_ad;
/*Buffer declaration variables */
  char buffer[65000];
char buffer_user[5000];
char readbuf[65000];
char writebuf[65000];
char writebuf_server[65000];
char writebuf_server2[65000];
char writebuf_ls[65000];

/*Variables for client server socket addressing */ 
  struct sockaddr_in serverAddr, clientAddr;
 struct sockaddr_in dest;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
 int status,socket_fd, client_fd,num;
    socklen_t size;

/*variables for count declaration of number of bytes sent or received*/
  int i,n=0,bytesReceived,bytesReceived2,bytesReceived_x,nuser=0,nuser_gf=0; 

/*variables for file name and command parsing and storage */
char * pch;
char * pch_user;
char * pch1;
char * pch1_user;
char * ser_nuser;
char server_nuser[200];char pch1_nuser[200];


/*Variables for opening and reading the contents of the server directory*/
DIR *d;
struct dirent *dir;

int nread;

   if ((clientSocket2 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
      printf("\nSocket failure!!\n");
    //    fprintf(stderr, "Socket failure!!\n");
    //    exit(1);
    }

 memset(&serverAddr, 0, sizeof(serverAddr));
memset(&dest,0,sizeof(dest));
  serverAddr.sin_family = AF_INET;
// serverAddr.sin_port = htons(atoi(argv[1])); //assigning the ip address of the client
 serverAddr.sin_port = htons((int)10002);
  serverAddr.sin_addr.s_addr = INADDR_ANY;//assigning any port number in accordance to the client


   if ((bind(clientSocket2, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))== -1)    { 
  printf("\nBind failure!!\n");      
//  fprintf(stderr, "Binding Failure\n");
     //   exit(1);
    }
 

/* **************listening for connections *********************** */
 if ((listen(clientSocket2, 10))== -1){
       printf("\nlisten failure!!\n");
  //      fprintf(stderr, "Listening Failure\n");
   //     exit(1);
    }
/******************************************************************/
  printf("\nprintf for check1!!\n");
/*zeroing the contents of buffers before alocation */
bzero(buffer,sizeof(buffer));   
bzero(buffer_user,sizeof(buffer_user)); 
bzero(buffer,sizeof(readbuf));
while(1)
{
  size = sizeof(struct sockaddr_in);
client_ad = sizeof(dest);
  printf("\nprintf for check2!!\n");
/* ************************** accepting connections ************************** */

 client_fd = accept(clientSocket2, (struct sockaddr*)&dest ,&client_ad);
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
/* ***************************************************************************** */
 if (client_fd<0)              //if there are negative request, print error
            error ("accept() error");
        else                        //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
while(1)  //while(1) characher infinite loop starting
{
/* ******************** login ************************************************ */
recv(client_fd,buffer_user,sizeof(buffer_user),0);
printf("\n the user is %s",buffer_user);

pch_user = strtok (buffer_user," "); //using string token to separate command and filename

strcpy(server_nuser,pch_user);
printf("User name pch is %s\n",server_nuser);

nuser++;
while (nuser<2)
{
pch1_user = strtok (NULL, " ");//using string token again to remove out new line characters
if (pch1_user==NULL)
{
break;
}
else
{
nuser++;
}
}
pch1_user=strtok(pch1_user,"\n"); 
printf("\npassword is %s",pch1_user);
strcpy(pch1_nuser,pch1_user);
printf("\npch1_nuser is %s",pch1_nuser);

FILE *pf=fopen("server_conf","r");
    char tmp[256]={0x0};
    while(pf!=NULL && fgets(tmp, sizeof(tmp), pf)!=NULL)
    {
        if (strstr(tmp,server_nuser))
{
         
               if (strstr(tmp,pch1_nuser))
{ 
           printf("\n%s", tmp);
printf("\n valid");
goto LOOP;
} 
else
{
printf("\n wrong password");
}
}
else
{
printf("\n User invalid");
}
   }
    if(pf!=NULL) fclose(pf);

exit(0);

LOOP:
nuser =0; //server_nuser[0] = '\0';
 pch1_nuser[0]='\0';buffer_user[0]='\0';
    memset(pch1_user,0,strlen(pch1_user));
 memset(buffer_user,0,strlen(buffer_user));
DIR *d;
  struct dirent *dir;
if (strcmp(server_nuser,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Alice";
chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(server_nuser,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Bob";
chdir (directory); 
printf("\n We are under Bob folder ");
}
char user_server[30];
strcpy(user_server,server_nuser);
 memset(server_nuser,0,strlen(server_nuser));
/* **************************************************************************** */

  printf("\nprintf for check3!!\n");
nread =0; writebuf[0] ='\0';nBytes3 = 0;readbuf[0] = '\0';//clearing the buffers and variables for next iteration

recv(client_fd,buffer,sizeof(buffer),0);

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
char filename[30];
strcpy(filename,pch1);

char STR1[30]; char STR2[30];
char PTR1[30]; char PTR2[30]; 
int str_1,str_2;
char folder[250];

/*function for put-client to server */
if(strcmp(pch,"put")==0)
{
  printf("\nprintf for check4!!\n");

/*buffer allocation */
char buffer_size1[50]; char packet_num1[4];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added

int status;

DIR* dir = opendir(pch1);
if(!dir)
{
status = mkdir(pch1, 0777);
}

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice Put folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob Put folder ");
}

/*receiving the number of bytes of the file to be sent and necessarily allocation buffer in chunks for that size */
recv(client_fd,buffer_size1,5,0);


puts(buffer_size1);
int bytesToRead = atoi(buffer_size1); //ascii to integer conversion for storing the buffer size in integer
int br=strlen(buffer_size1);
   printf("Bytes read  %d %d\n", bytesToRead,br);
int bytesby4 = bytesToRead / 4;
/*opening file in write mode to store the contents sent by the client*/

int num_x,num_y;
num_x = read(client_fd, packet_num1,1);
            printf("\npacket num1 %d %s\n",num_x, packet_num1);

int mod = atoi(packet_num1);

int needed = strlen(pch1) + 2;

char * store1 = (char *) malloc( sizeof( char ) * needed );
char * store2 = (char *) malloc( sizeof( char ) * needed );
if (mod == 0)
{

strcpy( store1, pch1);
strcat(store1,".2");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".3");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 1)
{

strcpy( store1, pch1);
strcat(store1,".1");
printf( "%s\n", store1 );
strcpy(STR1,store1);

strcpy( store2, pch1);
strcat(store2,".2");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 2)
{

strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 3)
{
strcpy( store1, pch1);
strcat(store1,".3");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}

strcpy(PTR1,STR1);
strcpy(PTR2,STR2);

FILE *fp;
        fp = fopen(store1, "wb"); 
        if(NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
       
        int totalbytesread=0;
int h,l;
char recvBuff[bytesby4+1];char recvBuff_x[bytesby4+1]; //char packet_num1[4]; //char packet_num2[4];

bytesReceived = read(client_fd, recvBuff,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived,recvBuff);
str_1=bytesReceived;
 for(h=0;recvBuff[h];h++)
     {
     recvBuff[h]=recvBuff[h]-3;
     }
fwrite(recvBuff, 1,bytesReceived,fp);
bytesReceived_x = read(client_fd, recvBuff_x,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived_x,recvBuff_x);
str_2=bytesReceived_x;
 for(l=0;recvBuff_x[l];l++)
     {
     recvBuff_x[l]=recvBuff_x[l]-3;
     }

        if(bytesReceived < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }
 if(bytesReceived_x < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
o = 0;
memset(buffer_size1,0,strlen(buffer_size1));
memset(recvBuff,0,strlen(recvBuff));
memset(packet_num1,0,strlen(packet_num1));
nread = 0; writebuf_server2[0] = '\0';bytesReceived=0;//bytesReceived_x=0;//bytesToRead=0;
totalbytesread=0;
num_x=0;num_y=0;
fclose(fp);//closing the file
//pch1[0]++;
FILE *p3;
p3 = fopen(store2,"wb");
  if(NULL == p3)
        {
            printf("Error opening file");
            return 1;
        }
  fwrite(recvBuff_x, 1,bytesReceived_x,p3);//write the data in a file to be stored at server local directory
fclose(p3);
memset(recvBuff_x,0,strlen(recvBuff_x));
bytesToRead=0;bytesReceived_x=0;bytesby4=0;
char *directory1 = "/home/swaminath/Downloads/split/pairs/server_thread/server2";
chdir (directory1); 
}

else if(strcmp(pch,"get")==0)
{
char buffer_size3[4000];char buffer_size[4000];
int bytesToWrite2=0,n=0,n2=0;int bytesToWrite3=0;
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}

/*opening file in read mode to send the contents of server to the client*/

FILE *input2 = fopen(STR1,"rb");
//FILE *input2 = fopen("client_file.txt.2","rb");  
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
//int nobytes = send(client_fd,buffer_size,strlen(buffer_size),0);
int nobytes = send(client_fd,buffer_size,5,0);
printf("\n nobytes is %d",nobytes); 
fclose(input2);//closing the file



//pch1[0]++;

FILE *input3 = fopen(STR2,"rb");
//FILE *input3 = fopen("client_file.txt.3","rb"); 
        if(input3==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(input3, 0, SEEK_END);
bytesToWrite3 = ftell(input3);//reading the size of the file 
printf("bytes to write is %d",bytesToWrite3);

int bytes3=(int)bytesToWrite3;
sprintf(buffer_size3,"%d",bytes3);//writing the size of the file in bytes to the client
puts(buffer_size3);
//send(client_fd,buffer_size3,strlen(buffer_size3),0);
send(client_fd,buffer_size3,5,0);
fclose(input3);//closing the file

/*opening the file in read mode*/
FILE* fp2 = fopen(STR1,"rb");
//FILE *fp2 = fopen("client_file.txt.2","rb");  
      if(fp2==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten2=0,r,q; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get2[bytesToWrite2+4];
 
       // int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);//reading the number of bytes sent
int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);        
printf("\nBytes read %d \n", nread2);  
         
 for(r=0;buff_get2[r];r++)
     {
     buff_get2[r]=buff_get2[r]+3;
     }

 int bytestowr2 = write(client_fd,buff_get2,nread2);
 printf("Bytes sent 3 %d %s\n",bytestowr2, buff_get2);
 
  
    
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get2,0,strlen(buff_get2));
memset(buffer_size,0,strlen(buffer_size));
nread2 = 0;
 bytes=0;
readbuf[0] = '\0';p=1;//bytesWritten2 = 0;n2=0;n=0;RTT=0;
bytesToWrite2 = 0;bytesWritten2=0;//m2=0;
bytestowr2=0;
fclose(fp2);//closing the file

/*opening the file in read mode*/
FILE* fp3 = fopen(STR2,"rb");
//FILE* fp3 = fopen("client_file.txt.3","rb");   
      if(fp3==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten3=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get3[bytesToWrite3+4];

//        int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);//reading the number of bytes sent
int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);        
printf("\nBytes read %d \n", nread3);  

 for(q=0;buff_get3[q];q++)
     {
     buff_get3[q]=buff_get3[q]+3;
     }
              
   int byteswr3 = write(client_fd,buff_get3,nread3);
 printf("Bytes sent 3 %d %s\n",byteswr3, buff_get3);

/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get3,0,strlen(buff_get3));
memset(buffer_size3,0,strlen(buffer_size3));
memset(STR1,0,strlen(STR1));
memset(STR2,0,strlen(STR2));
nread3=0; 
readbuf[0] = '\0';p=1;bytes3=0;
bytesToWrite2 = 0;bytesToWrite3 = 0;bytesWritten3=0;
fclose(fp3);//closing the file
char *directory2 = "/home/swaminath/Downloads/split/pairs/server_thread/server2";
chdir (directory2); 
}

else if (strcmp(pch,"ls")==0)
{
printf("\nfile name is %s",pch1);
printf("\n filename is %s",filename);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);
int status1;
if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n LS folder is %s",folder);
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server2/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}
/*directory variable initialization*/
  DIR *d;
  struct dirent *dir;
/*variables for reading the buffer and data packets */
int bytesToWrite3=0,n3=0;
char storage[256]; char seq_num3[2000];
char buffer_size4[50];
char buf_PTR1[str_1+1];
char buf_PTR2[str_2+1];
int bytesToWritex=0;int bytesToWritex1=0;
FILE *inputx = fopen(PTR1,"rb");
        if(inputx==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputx, 0, SEEK_END);
bytesToWritex = ftell(inputx);//reading the size of the file 
printf("bytes to write is %d",bytesToWritex);
fclose(inputx);

FILE *inputy = fopen(PTR2,"rb");
        if(inputy==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputy, 0, SEEK_END);
bytesToWritex1 = ftell(inputy);//reading the size of the file 
printf("bytes to write x1 is %d",bytesToWritex1);
fclose(inputy);

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
if(bytesToWritex1 != str_2)
{
if(strcmp(dir->d_name,PTR2)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 

if(bytesToWritex != str_1)
{
if(strcmp(dir->d_name,PTR1)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 
 
//printf("%s [Incomplete]\n", dir->d_name);

else
{
     printf("%s\n", dir->d_name);
fprintf(fp4, "%s\n",dir->d_name);
}

 //file write the contents
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

send(client_fd,buffer_size4,3,0);
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
    
 if(nread > 0)
 {
            printf("Sending: %s\n",buff_ls);
send(client_fd,buff_ls,nread,0);
bytesWritten3+=m3;  
}
 
}       
          
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buffer_size4,0,strlen(buffer_size4));
nread =0; writebuf_ls[0] ='\0';bytesWritten3=0,bytesToWrite3=0;r=1;
fclose(input);//closing the file
    closedir(d);//closing the current directory
  }
char *directory3 = "/home/swaminath/Downloads/split/pairs/server_thread/server2";
chdir (directory3);
}

server_nuser[0] = '\0';
memset(server_nuser,0,strlen(server_nuser));
}

}
}
  //Close Connection Socket
        close(client_fd);
    } //Outer While
pthread_mutex_unlock(&msgSem2);
 // return 0;
sleep(10);
}



/*Main function with two arguments-ip address and the port number */
void *server3(void *args){
sleep(15);
 pthread_mutex_lock(&msgSem3);
 int clientSocket3, nBytes,nBytes3,o=0;
  int yes =1;int client_ad;
/*Buffer declaration variables */
  char buffer[65000];
char buffer_user[5000];
char readbuf[65000];
char writebuf[65000];
char writebuf_server[65000];
char writebuf_server2[65000];
char writebuf_ls[65000];

/*Variables for client server socket addressing */ 
  struct sockaddr_in serverAddr, clientAddr;
 struct sockaddr_in dest;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
 int status,socket_fd, client_fd,num;
    socklen_t size;

/*variables for count declaration of number of bytes sent or received*/
int i,n=0,bytesReceived,bytesReceived2,bytesReceived_x,nuser=0,nuser_gf=0; 

/*variables for file name and command parsing and storage */
char * pch;
char * pch_user;
char * pch1;
char * pch1_user;
char * ser_nuser;
char server_nuser[200];char pch1_nuser[200];

/*Variables for opening and reading the contents of the server directory*/
DIR *d;
struct dirent *dir;

int nread;

/*For checking the number of arguments at the server end at run time*/
/*
if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}
*/
/*Creating a socket at the server end*/
 // clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   if ((clientSocket3 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
      printf("\nSocket failure!!\n");
   //     fprintf(stderr, "Socket failure!!\n");
   //     exit(1);
    }

 memset(&serverAddr, 0, sizeof(serverAddr));
memset(&dest,0,sizeof(dest));
  serverAddr.sin_family = AF_INET;
// serverAddr.sin_port = htons(atoi(argv[1])); //assigning the ip address of the client
 serverAddr.sin_port = htons(10003);
  serverAddr.sin_addr.s_addr = INADDR_ANY;//assigning any port number in accordance to the client


   if ((bind(clientSocket3, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))== -1)    { 
  printf("\nBind failure!!\n");      
 // fprintf(stderr, "Binding Failure\n");
     //   exit(1);
    }
 // bind(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//binding the socket

 // addr_size = sizeof serverStorage; //reading the address size


/* **************listening for connections *********************** */
 if ((listen(clientSocket3, 10))== -1){
       printf("\nlisten failure!!\n");
     //   fprintf(stderr, "Listening Failure\n");
     //   exit(1);
    }
/******************************************************************/
  printf("\nprintf for check1!!\n");
/*zeroing the contents of buffers before alocation */
bzero(buffer,sizeof(buffer));   
bzero(buffer_user,sizeof(buffer_user)); 
bzero(buffer,sizeof(readbuf));
while(1)
{
  size = sizeof(struct sockaddr_in);
client_ad = sizeof(dest);
  printf("\nprintf for check2!!\n");
/* ************************** accepting connections ************************** */
 client_fd = accept(clientSocket3, (struct sockaddr*)&dest ,&client_ad);
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
/* ***************************************************************************** */
 if (client_fd<0)              //if there are negative request, print error
            error ("accept() error");
        else                        //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
while(1)  //while(1) characher infinite loop starting
{

/* ********************** login ************************************************** */
recv(client_fd,buffer_user,sizeof(buffer_user),0);
printf("\n the user is %s",buffer_user); 

pch_user = strtok (buffer_user," "); //using string token to separate command and filename

strcpy(server_nuser,pch_user);
printf("User name pch is %s\n",server_nuser);

nuser++;
while (nuser<2)
{
pch1_user = strtok (NULL, " ");//using string token again to remove out new line characters
if (pch1_user==NULL)
{
break;
}
else
{
nuser++;
}
}
pch1_user=strtok(pch1_user,"\n"); 
printf("\npassword is %s",pch1_user);
strcpy(pch1_nuser,pch1_user);
printf("\npch1_nuser is %s",pch1_nuser);

FILE *pf=fopen("server_conf","r");
    char tmp[256]={0x0};
    while(pf!=NULL && fgets(tmp, sizeof(tmp), pf)!=NULL)
    {
        if (strstr(tmp,server_nuser))
{
         
               if (strstr(tmp,pch1_nuser))
{ 
           printf("\n%s", tmp);
printf("\n valid");
goto LOOP;
} 
else
{
printf("\n wrong password");
}
}
else
{
printf("\n User invalid");
}
   }
    if(pf!=NULL) fclose(pf);

exit(0);

LOOP:
nuser =0; //server_nuser[0] = '\0';
 pch1_nuser[0]='\0';buffer_user[0]='\0';
    memset(pch1_user,0,strlen(pch1_user));
 memset(buffer_user,0,strlen(buffer_user));
DIR *d;
  struct dirent *dir;
if (strcmp(server_nuser,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Alice";
chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(server_nuser,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Bob";
chdir (directory); 
printf("\n We are under Bob folder ");
}

char user_server[30];
strcpy(user_server,server_nuser);
 memset(server_nuser,0,strlen(server_nuser));
/* *************************************************************************** */

  printf("\nprintf for check3!!\n");
nread =0; writebuf[0] ='\0';nBytes3 = 0;readbuf[0] = '\0';//clearing the buffers and variables for next iteration

recv(client_fd,buffer,sizeof(buffer),0);

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


char filename[30];
strcpy(filename,pch1);


char STR1[30]; char STR2[30];
char PTR1[30]; char PTR2[30]; 
int str_1,str_2;
char folder[250];

/*function for put-client to server */
if(strcmp(pch,"put")==0)
{
  printf("\nprintf for check4!!\n");

/*buffer allocation */
char buffer_size1[50]; char packet_num1[4];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added

int status;

DIR* dir = opendir(pch1);
if(!dir)
{
status = mkdir(pch1, 0777);
}

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice Put folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob Put folder ");
}
/*receiving the number of bytes of the file to be sent and necessarily allocation buffer in chunks for that size */
recv(client_fd,buffer_size1,5,0);


puts(buffer_size1);
int bytesToRead = atoi(buffer_size1); //ascii to integer conversion for storing the buffer size in integer
int br=strlen(buffer_size1);
   printf("Bytes read  %d %d\n", bytesToRead,br);
int bytesby4 = bytesToRead / 4;
/*opening file in write mode to store the contents sent by the client*/


int num_x,num_y;
num_x = read(client_fd, packet_num1,1);
            printf("\npacket num1 %d %s\n",num_x, packet_num1);

int mod = atoi(packet_num1);
int needed = strlen(pch1) + 2;

char * store1 = (char *) malloc( sizeof( char ) * needed );
char * store2 = (char *) malloc( sizeof( char ) * needed );

if (mod == 0)
{

strcpy( store1, pch1);
strcat(store1,".3");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 1)
{

strcpy( store1, pch1);
strcat(store1,".2");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".3");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 2)
{
strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".2");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 3)
{
strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
strcpy(PTR1,STR1);
strcpy(PTR2,STR2);

FILE *fp;
        fp = fopen(store1, "wb"); 
        if(NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
       
        int totalbytesread=0;

char recvBuff[bytesby4+1];char recvBuff_x[bytesby4+1]; //char packet_num1[4]; //char packet_num2[4];
int h,l;

bytesReceived = read(client_fd, recvBuff,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived,recvBuff);
str_1=bytesReceived;
 for(h=0;recvBuff[h];h++)
     {
     recvBuff[h]=recvBuff[h]-3;
     }
fwrite(recvBuff, 1,bytesReceived,fp);
bytesReceived_x = read(client_fd, recvBuff_x,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived_x,recvBuff_x);
str_2=bytesReceived_x;
for(l=0;recvBuff_x[l];l++)
     {
     recvBuff_x[l]=recvBuff_x[l]-3;
     }

        if(bytesReceived < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }
 if(bytesReceived_x < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */
o = 0;
memset(buffer_size1,0,strlen(buffer_size1));
memset(recvBuff,0,strlen(recvBuff));
memset(packet_num1,0,strlen(packet_num1));

nread = 0; writebuf_server2[0] = '\0';bytesReceived=0;//bytesReceived_x=0;//bytesToRead=0;
totalbytesread=0;
num_x=0;num_y=0;
fclose(fp);//closing the file
pch1[0]++;
FILE *p3;
p3 = fopen(store2,"wb");
  if(NULL == p3)
        {
            printf("Error opening file");
            return 1;
        }
  fwrite(recvBuff_x, 1,bytesReceived_x,p3);//write the data in a file to be stored at server local directory
fclose(p3);
memset(recvBuff_x,0,strlen(recvBuff_x));
bytesToRead=0;bytesReceived_x=0;bytesby4=0;
char *directory1 = "/home/swaminath/Downloads/split/pairs/server_thread/server3";
chdir (directory1); 
}

else if(strcmp(pch,"get")==0)
{
char buffer_size3[4000];char buffer_size[4000];
int bytesToWrite2=0,n=0,n2=0;int bytesToWrite3=0;
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}

/*opening file in read mode to send the contents of server to the client*/

FILE *input2 = fopen(STR1,"rb");
//FILE *input2 = fopen("client_file.txt.3","rb"); 
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

int nobytes =send(client_fd,buffer_size,5,0);
printf("\n nobytes is %d",nobytes); 
fclose(input2);//closing the file

FILE *input3 = fopen(STR2,"rb");
//FILE *input3 = fopen("client_file.txt.4","rb"); 
        if(input3==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(input3, 0, SEEK_END);
bytesToWrite3 = ftell(input3);//reading the size of the file 
printf("bytes to write is %d",bytesToWrite3);

int bytes3=(int)bytesToWrite3;
sprintf(buffer_size3,"%d",bytes3);//writing the size of the file in bytes to the client
puts(buffer_size3);
//send(client_fd,buffer_size3,strlen(buffer_size3),0);
send(client_fd,buffer_size3,5,0);
fclose(input3);//closing the file

/*opening the file in read mode*/
FILE* fp2 = fopen(STR1,"rb");
//FILE* fp2 = fopen("client_file.txt.3","rb"); 
        if(fp2==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten2=0,r,q; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get2[bytesToWrite2+4];
 
       // int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);//reading the number of bytes sent
int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);        
printf("\nBytes read %d \n", nread2);  
         
 for(r=0;buff_get2[r];r++)
     {
     buff_get2[r]=buff_get2[r]+3;
     }
 int bytestowr2 = write(client_fd,buff_get2,nread2);
 printf("Bytes sent 3 %d %s\n",bytestowr2, buff_get2);
     
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get2,0,strlen(buff_get2));
memset(buffer_size,0,strlen(buffer_size));
nread2 = 0;
 bytes=0;
readbuf[0] = '\0';p=1;//bytesWritten2 = 0;n2=0;n=0;RTT=0;
bytesToWrite2 = 0;bytesWritten2=0;//m2=0;
bytestowr2=0;
fclose(fp2);//closing the file

pch1[0]++;

/*opening the file in read mode*/
FILE* fp3 = fopen(STR2,"rb");
//FILE* fp3 = fopen("client_file.txt.4","rb"); 
        if(fp3==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten3=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get3[bytesToWrite3+4];

//        int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);//reading the number of bytes sent
int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);        
printf("\nBytes read %d \n", nread3);  
              
 for(q=0;buff_get3[q];q++)
     {
     buff_get3[q]=buff_get3[q]+3;
     }
   int byteswr3 = write(client_fd,buff_get3,nread3);
 printf("Bytes sent 3 %d %s\n",byteswr3, buff_get3);

/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get3,0,strlen(buff_get3));
memset(buffer_size3,0,strlen(buffer_size3));
memset(STR1,0,strlen(STR1));
memset(STR2,0,strlen(STR2));
nread3=0; 
readbuf[0] = '\0';p=1;bytes3=0;
bytesToWrite2 = 0;bytesToWrite3 = 0;bytesWritten3=0;
fclose(fp3);//closing the file
char *directory2 = "/home/swaminath/Downloads/split/pairs/server_thread/server3";
chdir (directory2); 
}

else if (strcmp(pch,"ls")==0)
{
printf("\nfile name is %s",pch1);
printf("\n filename is %s",filename);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);
int status1;
if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n LS folder is %s",folder);
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server3/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}
/*directory variable initialization*/
  DIR *d;
  struct dirent *dir;
/*variables for reading the buffer and data packets */
int bytesToWrite3=0,n3=0;
char storage[256]; char seq_num3[2000];
char buffer_size4[50];
char buf_PTR1[str_1+1];
char buf_PTR2[str_2+1];
int bytesToWritex=0;int bytesToWritex1=0;
FILE *inputx = fopen(PTR1,"rb");
        if(inputx==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputx, 0, SEEK_END);
bytesToWritex = ftell(inputx);//reading the size of the file 
printf("bytes to write is %d",bytesToWritex);
fclose(inputx);

FILE *inputy = fopen(PTR2,"rb");
        if(inputy==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputy, 0, SEEK_END);
bytesToWritex1 = ftell(inputy);//reading the size of the file 
printf("bytes to write x1 is %d",bytesToWritex1);
fclose(inputy);
//int PTR1_read = fread(buf_PTR1,1,str_1,inputx);
/*
if(bytesToWritex != str_1)
{
strcat(PTR1," [Incomplete]");
}
else if(strstr(PTR1," [Incomplete]"))
{

}
*/
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
if(bytesToWritex1 != str_2)
{
if(strcmp(dir->d_name,PTR2)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 

if(bytesToWritex != str_1)
{
if(strcmp(dir->d_name,PTR1)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
}
//printf("%s [Incomplete]\n", dir->d_name);

else
{
     printf("%s\n", dir->d_name);
fprintf(fp4, "%s\n",dir->d_name);
}

 //file write the contents
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

send(client_fd,buffer_size4,3,0);
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
    
 if(nread > 0)
 {
            printf("Sending: %s\n",buff_ls);
send(client_fd,buff_ls,nread,0);
bytesWritten3+=m3;  
}
 
}       
          
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buffer_size4,0,strlen(buffer_size4));
nread =0; writebuf_ls[0] ='\0';bytesWritten3=0,bytesToWrite3=0;r=1;
fclose(input);//closing the file
    closedir(d);//closing the current directory
  }
char *directory3 = "/home/swaminath/Downloads/split/pairs/server_thread/server3";
chdir (directory3);
}


server_nuser[0] = '\0';
 memset(server_nuser,0,strlen(server_nuser));
}

}
}
  //Close Connection Socket
        close(client_fd);
    } //Outer While
 pthread_mutex_unlock(&msgSem3);
sleep(10);
}





/*Main function with two arguments-ip address and the port number */
void *server4(void *args)
{
sleep(20);

pthread_mutex_lock(&msgSem4);
 int clientSocket4, nBytes,nBytes3,o=0;
  int yes =1;int client_ad;
/*Buffer declaration variables */
  char buffer[65000];
char buffer_user[5000];
char readbuf[65000];
char writebuf[65000];
char writebuf_server[65000];
char writebuf_server2[65000];
char writebuf_ls[65000];

/*Variables for client server socket addressing */ 
  struct sockaddr_in serverAddr, clientAddr;
 struct sockaddr_in dest;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
 int status,socket_fd, client_fd,num;
    socklen_t size;

/*variables for count declaration of number of bytes sent or received*/
 int i,n=0,bytesReceived,bytesReceived2,bytesReceived_x,nuser=0,nuser_gf=0; 

/*variables for file name and command parsing and storage */
char * pch;
char * pch_user;
char * pch1;
char * pch1_user;
char * ser_nuser;
char server_nuser[200];char pch1_nuser[200];

/*Variables for opening and reading the contents of the server directory*/
DIR *d;
struct dirent *dir;

int nread;

   if ((clientSocket4 = socket(AF_INET, SOCK_STREAM, 0))== -1) {
      printf("\nSocket failure!!\n");
   //     fprintf(stderr, "Socket failure!!\n");
   //     exit(1);
    }

 memset(&serverAddr, 0, sizeof(serverAddr));
memset(&dest,0,sizeof(dest));
  serverAddr.sin_family = AF_INET;
// serverAddr.sin_port = htons(atoi(argv[1])); //assigning the ip address of the client
 serverAddr.sin_port = htons(10004);
  serverAddr.sin_addr.s_addr = INADDR_ANY;//assigning any port number in accordance to the client


   if ((bind(clientSocket4, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))== -1)    { 
  printf("\nBind failure!!\n");      
    }



/* **************listening for connections *********************** */
 if ((listen(clientSocket4, 10))== -1){
       printf("\nlisten failure!!\n");
    }
/******************************************************************/
  printf("\nprintf for check1!!\n");
/*zeroing the contents of buffers before alocation */
bzero(buffer,sizeof(buffer));   
bzero(buffer_user,sizeof(buffer_user)); 
bzero(buffer,sizeof(readbuf));
while(1)
{
  size = sizeof(struct sockaddr_in);
client_ad = sizeof(dest);
  printf("\nprintf for check2!!\n");
/* ************************** accepting connections ************************** */
client_fd = accept(clientSocket4, (struct sockaddr*)&dest ,&client_ad);
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
/* ***************************************************************************** */
 if (client_fd<0)              //if there are negative request, print error
            error ("accept() error");
        else                        //otherwise do forking to handle multiple simultaneous requests
        {
            if ( fork()==0 )
            {
while(1)  //while(1) characher infinite loop starting
{

/* *****************************login *************************************** */
recv(client_fd,buffer_user,sizeof(buffer_user),0);
printf("\n the user is %s",buffer_user); 

pch_user = strtok (buffer_user," "); //using string token to separate command and filename

strcpy(server_nuser,pch_user);
printf("User name pch is %s\n",server_nuser);

nuser++;
while (nuser<2)
{
pch1_user = strtok (NULL, " ");//using string token again to remove out new line characters
if (pch1_user==NULL)
{
break;
}
else
{
nuser++;
}
}
pch1_user=strtok(pch1_user,"\n"); 
printf("\npassword is %s",pch1_user);
strcpy(pch1_nuser,pch1_user);
printf("\npch1_nuser is %s",pch1_nuser);

FILE *pf=fopen("server_conf","r");
    char tmp[256]={0x0};
    while(pf!=NULL && fgets(tmp, sizeof(tmp), pf)!=NULL)
    {
        if (strstr(tmp,server_nuser))
{
         
               if (strstr(tmp,pch1_nuser))
{ 
           printf("\n%s", tmp);
printf("\n valid");
goto LOOP;
} 
else
{
printf("\n wrong password");
}
}
else
{
printf("\n User invalid");
}
   }
    if(pf!=NULL) fclose(pf);

exit(0);

LOOP:
nuser =0; //server_nuser[0] = '\0';
 pch1_nuser[0]='\0';buffer_user[0]='\0';
    memset(pch1_user,0,strlen(pch1_user));
 memset(buffer_user,0,strlen(buffer_user));
DIR *d;
  struct dirent *dir;
if (strcmp(server_nuser,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Alice";
chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(server_nuser,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Bob";
chdir (directory); 
printf("\n We are under Bob folder ");
}
char user_server[30];
strcpy(user_server,server_nuser);
 memset(server_nuser,0,strlen(server_nuser));
/* **************************************************************************** */

  printf("\nprintf for check3!!\n");
nread =0; writebuf[0] ='\0';nBytes3 = 0;readbuf[0] = '\0';//clearing the buffers and variables for next iteration

recv(client_fd,buffer,sizeof(buffer),0);

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

char filename[30];
strcpy(filename,pch1);


char STR1[30]; char STR2[30];
char PTR1[30]; char PTR2[30]; 
int str_1,str_2;
char folder[250];
/*function for put-client to server */
if(strcmp(pch,"put")==0)
{
  printf("\nprintf for check4!!\n");

/*buffer allocation */
char buffer_size1[50]; char packet_num1[4];
printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added

int status;

DIR* dir = opendir(pch1);
if(!dir)
{
status = mkdir(pch1, 0777);
}

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice Put folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob Put folder ");
}
/*receiving the number of bytes of the file to be sent and necessarily allocation buffer in chunks for that size */
recv(client_fd,buffer_size1,5,0);


puts(buffer_size1);
int bytesToRead = atoi(buffer_size1); //ascii to integer conversion for storing the buffer size in integer
int br=strlen(buffer_size1);
   printf("Bytes read  %d %d\n", bytesToRead,br);
int bytesby4 = bytesToRead / 4;

int num_x,num_y;
num_x = read(client_fd, packet_num1,1);
            printf("\npacket num1 %d %s\n",num_x, packet_num1);

int mod = atoi(packet_num1);
int needed = strlen(pch1) + 2;

char * store1 = (char *) malloc( sizeof( char ) * needed );
char * store2 = (char *) malloc( sizeof( char ) * needed );

if (mod == 0)
{

strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 1)
{

strcpy( store1, pch1);
strcat(store1,".3");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".4");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 2)
{
strcpy( store1, pch1);
strcat(store1,".2");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".3");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
else if (mod == 3)
{

strcpy( store1, pch1);
strcat(store1,".1");
strcpy(STR1,store1);
printf( "%s\n", store1 );

strcpy( store2, pch1);
strcat(store2,".2");
strcpy(STR2,store2);
printf( "%s\n", store2 );
}
strcpy(PTR1,STR1);
strcpy(PTR2,STR2);

FILE *fp;
        fp = fopen(store2, "wb"); 
        if(NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
       
        int totalbytesread=0;

char recvBuff[bytesby4+1];char recvBuff_x[bytesby4+1]; //char packet_num1[4]; //char packet_num2[4];
int h,l;
bytesReceived = read(client_fd, recvBuff,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived,recvBuff);
str_1=bytesReceived;
 for(h=0;recvBuff[h];h++)
     {
     recvBuff[h]=recvBuff[h]-3;
     }

bytesReceived_x = read(client_fd, recvBuff_x,bytesby4+1);
            printf("\nBytes received %d %s\n",bytesReceived_x,recvBuff_x);
str_2=bytesReceived_x;
for(l=0;recvBuff_x[l];l++)
     {
     recvBuff_x[l]=recvBuff_x[l]-3;
     }
fwrite(recvBuff_x, 1,bytesReceived_x,fp);
        if(bytesReceived < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }
 if(bytesReceived_x < 0)   //if you haven't received any data, report a read error 
        {
            printf("\n Read Error \n");
        }

/*clearing the variables for next iteration */

fclose(fp);//closing the file
//pch1[0]++;
FILE *p3;
p3 = fopen(store1,"wb");
  if(NULL == p3)
        {
            printf("Error opening file");
            return 1;
        }
  fwrite(recvBuff, 1,bytesReceived,p3);//write the data in a file to be stored at server local directory
fclose(p3);
memset(recvBuff,0,strlen(recvBuff));
bytesToRead=0;bytesReceived=0;bytesby4=0;
memset(buffer_size1,0,strlen(buffer_size1));
memset(recvBuff_x,0,strlen(recvBuff_x));
memset(packet_num1,0,strlen(packet_num1));
nread = 0; writebuf_server2[0] = '\0';bytesReceived_x=0;//bytesReceived_x=0;//bytesToRead=0;
totalbytesread=0;
num_x=0;num_y=0;
char *directory1 = "/home/swaminath/Downloads/split/pairs/server_thread/server4";
chdir (directory1); 
}

else if(strcmp(pch,"get")==0)
{
char buffer_size3[4000];char buffer_size[4000];
int bytesToWrite2=0,n=0,n2=0;int bytesToWrite3=0;
char seq_num1[2000];

printf("\nfile name is %s",pch1);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);

if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
//chdir (directory); 
printf("\n We are under Bob folder ");
}
/*opening file in read mode to send the contents of server to the client*/

FILE *input2 = fopen(STR1,"rb");
//FILE *input2 = fopen("client_file.txt.4","rb"); 
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
//int nobytes = send(client_fd,buffer_size,strlen(buffer_size),0);
int nobytes = send(client_fd,buffer_size,5,0);
printf("\n nobytes is %d",nobytes); 
fclose(input2);//closing the file


//pch1[0]++;

FILE *input3 = fopen(STR2,"rb");
//FILE *input3 = fopen("client_file.txt.1","rb"); 
        if(input3==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(input3, 0, SEEK_END);
bytesToWrite3 = ftell(input3);//reading the size of the file 
printf("bytes to write is %d",bytesToWrite3);

int bytes3=(int)bytesToWrite3;
sprintf(buffer_size3,"%d",bytes3);//writing the size of the file in bytes to the client
puts(buffer_size3);
send(client_fd,buffer_size3,5,0);
fclose(input3);//closing the file

/*opening the file in read mode*/
FILE* fp2 = fopen(STR1,"rb");
//FILE* fp2 = fopen("client_file.txt.4","rb");   
      if(fp2==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten2=0,r,q; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get2[bytesToWrite2+4];
 
       // int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);//reading the number of bytes sent
int nread2 = fread(buff_get2,1,bytesToWrite2,fp2);        
printf("\nBytes read %d \n", nread2);  
 for(r=0;buff_get2[r];r++)
     {
     buff_get2[r]=buff_get2[r]+3;
     }
         
 int bytestowr2 = write(client_fd,buff_get2,nread2);
 printf("Bytes sent 3 %d %s\n",bytestowr2, buff_get2);
 
  
    
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get2,0,strlen(buff_get2));
memset(buffer_size,0,strlen(buffer_size));
nread2 = 0;
 bytes=0;
readbuf[0] = '\0';p=1;//bytesWritten2 = 0;n2=0;n=0;RTT=0;
bytesToWrite2 = 0;bytesWritten2=0;//m2=0;
bytestowr2=0;
fclose(fp2);//closing the file


/*opening the file in read mode*/
FILE* fp3 = fopen(STR2,"rb");
//FILE* fp3 = fopen("client_file.txt.1","rb"); 
        if(fp3==NULL)
        {
            printf("File open error");
           return 1;   
        }

int bytesWritten3=0; 

/*comparing whether the all the contents of the file to be sent to the client are read */


   unsigned char buff_get3[bytesToWrite3+4];

int nread3 = fread(buff_get3,1,bytesToWrite3,fp3);        
printf("\nBytes read %d \n", nread3);  
              
 for(q=0;buff_get3[q];q++)
     {
     buff_get3[q]=buff_get3[q]+3;
     }
   int byteswr3 = write(client_fd,buff_get3,nread3);
 printf("Bytes sent 3 %d %s\n",byteswr3, buff_get3);

/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buff_get3,0,strlen(buff_get3));
memset(buffer_size3,0,strlen(buffer_size3));
memset(STR1,0,strlen(STR1));
memset(STR2,0,strlen(STR2));
nread3=0; 
readbuf[0] = '\0';p=1;bytes3=0;
bytesToWrite2 = 0;bytesToWrite3 = 0;bytesWritten3=0;
fclose(fp3);//closing the file
char *directory2 = "/home/swaminath/Downloads/split/pairs/server_thread/server4";
chdir (directory2); 
}

else if (strcmp(pch,"ls")==0)
{
printf("\nfile name is %s",pch1);
printf("\n filename is %s",filename);
pch1=strtok(pch1,"\n"); //using string token to remove out new line characters thatI might be added
printf("\nCommand is %s\n", pch);
int status1;
if (strcmp(user_server,"Alice")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Alice";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
printf("\n LS folder is %s",folder);
printf("\n We are under Alice folder ");
}
else if(strcmp(user_server,"Bob")==0)
{
char *directory = "/home/swaminath/Downloads/split/pairs/server_thread/server4/Bob";
strcpy(folder,directory);
strcat(folder,"/");
strcat(folder,pch1);
chdir (folder);
printf("\n We are under Bob folder ");
}
/*directory variable initialization*/
  DIR *d;
  struct dirent *dir;
/*variables for reading the buffer and data packets */
int bytesToWrite3=0,n3=0;
char storage[256]; char seq_num3[2000];
char buffer_size4[50];
char buf_PTR1[str_1+1];
char buf_PTR2[str_2+1];
int bytesToWritex=0;int bytesToWritex1=0;
FILE *inputx = fopen(PTR1,"rb");
        if(inputx==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputx, 0, SEEK_END);
bytesToWritex = ftell(inputx);//reading the size of the file 
printf("bytes to write is %d",bytesToWritex);
fclose(inputx);

FILE *inputy = fopen(PTR2,"rb");
        if(inputy==NULL)
        {
            printf("File open error");
            return 1;   
        } 
 fseek(inputy, 0, SEEK_END);
bytesToWritex1 = ftell(inputy);//reading the size of the file 
printf("bytes to write x1 is %d",bytesToWritex1);
fclose(inputy);

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
if(bytesToWritex1 != str_2)
{
if(strcmp(dir->d_name,PTR2)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 

if(bytesToWritex != str_1)
{
if(strcmp(dir->d_name,PTR1)==0)
{
printf("%s [Incomplete]\n", dir->d_name);
fprintf(fp4, "%s [Incomplete]\n",dir->d_name);
}
} 
 
//printf("%s [Incomplete]\n", dir->d_name);

else
{
     printf("%s\n", dir->d_name);
fprintf(fp4, "%s\n",dir->d_name);
}
	
 //file write the contents
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

send(client_fd,buffer_size4,3,0);
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
    
 if(nread > 0)
 {
            printf("Sending: %s\n",buff_ls);
send(client_fd,buff_ls,nread,0);
bytesWritten3+=m3;  
}
 
}       
          
/*clearing the variables for next iteration */
memset(readbuf,0,strlen(readbuf));
memset(buffer_size4,0,strlen(buffer_size4));
nread =0; writebuf_ls[0] ='\0';bytesWritten3=0,bytesToWrite3=0;r=1;
fclose(input);//closing the file
    closedir(d);//closing the current directory
  }
char *directory3 = "/home/swaminath/Downloads/split/pairs/server_thread/server4";
chdir (directory3);
}


server_nuser[0] = '\0';
memset(server_nuser,0,strlen(server_nuser));

}
}
}
  //Close Connection Socket
        close(client_fd);

    } //Outer While
 pthread_mutex_unlock(&msgSem4);
sleep(10);
}
int main()
{
pthread_t t_10,t_20,t_30,t_40;


	pthread_create (&t_10, NULL, server1,(void *)0);
	pthread_create (&t_20, NULL, server2,(void *)0);
pthread_create (&t_30, NULL, server3,(void *)0);
	pthread_create (&t_40, NULL, server4,(void *)0);

pthread_exit (NULL);
 exit(0);
}

//  return 0;







