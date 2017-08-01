1. In this programming assignment, I have created two files- client.c and server.c and with those, I'm able to perform tasks within the same host ip address and port numbers above 5000 like
i)Sending a file from client to server under 'put' command
ii)Server to client under 'get', 
iii)Serving reading the contents of its directory and sending the contents to the client under 'ls' command
iv)Exiting the process of both the server and the client under 'exit' command

2. I'm able to test FOO1, FOO2 and FOO3 files with my algorithm and all worked fine even before implementing reliability protocol.

3. I'm using UDP protocol for this data communication between the client and the server and as UDP is a connection-less protocol, it's also insecure and some reliability mechanism must be implemented to avoid packet losses for large files. So I have used Stop-and-Wait mechanism to avoid packet losses for large files.

4. Under Stop-and-Wait mechanism, the sender sends a packet with a sequence number to the receiver. The receiver,once received the packet, will check the packet and also for the whether it has received the correct packet by verifying the sequence number sent by the sender. If the sequence number as well as the data message received are correct, then it will provide an acknowledgement for the packet. This continues for all the packets. Sender will have to wait for the acknowledgement under some timeout period. I have given a timeout period of 10 milliseconds. If the acknowledgement hasn't arrived by that time, the sender resends the packet with the same sequence number. Reciever will ignore the lost packet frame or the one with the wrong sequence number. With this approach, if there are packet losses, the data can be reliably transferred to the receiver, but at a little slower pace than without losses. 

5. For calculating the timeout and RTT, I have defined a struct timespec variable and utilized timer.h for including the structure variables. This will provide timer period of the order nanoseconds. I have been able to get a RTT value of the order microseconds for data transfer within the same host network and I verified this with the ping on 127.0.0.1.

6. I do have two executables, client and server and with the makefile named makefile. I have created a single makefile and by running that, both the executables will be compiled and obtained. Makefile can be compiled with the command make, which complied and links all the changes and creates two executables, client and server

7. Execution procedure
./server port_number(>5000)

./client host_ip_address port_number(>5000)

Preferably, use the same port number for both.
command filename

example - for get foo1    (or) put foo2
incase of 'ls' and 'exit', a space must be added after typing the command to execute

8. I would like to thank Professor Sangthe Ha for giving this deadline extension and also thank all the TA's for their support for helping in clarifying my doubts.

9. If there are line numbers added by default by the test editor, I would request you to remove that before reading as, I have added my own point numbers and I thank you for following the same.
