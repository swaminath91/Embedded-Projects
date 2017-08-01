Information about my project
1. I have completed this distributed file system along with extra credit of traffic optimization, where the GET command is optimized to receive only one copy of data and duplicate is neglected. I have first received information about the file size of all 8 pieces from servers and based on the md5sum % 4 value, I group them accordingly and check for whether the piece received is of proper size and encoding. If first piece is successful, then the duplicate will be neglected and will not be received at all. This saves space at the client and the client will not be overloaded

2. I have created a function for finding the MD5sum of the file and doing modulus with 4 and accordingly sending pairs to each servers.

3. I have stored information about servers in the client_conf config file. I read information about ip address(localhost) and port numbers for each server from the file

4. I have stored login information for servers in server_conf config file, which verifies the login credentials with one in the config file

5. I have created subfolders for each command say get, put and ls

6. I have used a caesar cipher encryption with the password in the config file. Server will have one password at its end. Server can decrypt only if its password matches with the one client sends

7. I have performed this assignment in two methods, first method is with one client file and four individual scripts for running each server. Second method is using pthreads to combine all four servers into one driver program. Second method integrates all four servers with pthreads and all works with list, ls, get and put.

8. Functions implemented
put - splitting a file into 4 pieces and sending to each server and storing in the subfolder that user requests
get- receiving four individual pieces from 4 servers and combining them at the client. Duplicates are avoided and traffic optimization is implemented
ls- reads files stored at server subfolders and sends them to the client
list- just displays whether the file is complete or incomplete and can be recovered or not

9. I have used fork to handle multiple simultaneous client requests and I have tested it working

10. I have tested for whether server responds in a second and all work.





