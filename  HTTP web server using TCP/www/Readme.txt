1. In this programming assignment, I have created a HTTP webserver with TCP and I'm able to handle multiple requests from the client at a time. I have used C programming for my whole assignment. I have used forking to handle multiple simultanoeus requests at a time

2.I have handled all the error conditions requested like HTTP 200(present,not error), 404, 400, 501 and 500

3. I have used a configuration file named ws_conf to read all the data like file location, server port number, data types supported and connection type-keep-alive status and timeout period. I store all the data read into several buffers and structure to carefully handle every request from the user

4.Every information say will be sent to the webpage client, whether the file is present, or not , or unsupported

5. I have implemented both the extra-credit requirements- pipelining and HTTP POST request

6. For HTTP POST request, I have created a html script named login.html and I created a simple webpage containg two icons prompting user to enter user name and paswword. Upon user entering the details on the webpage, a response message will be printed on the server terminal about the request and what user entered as values will be visible under the body of the POST response in the terminal

7. For the pipelining extra-credit requirment, I have tested pipelining for two clients. One for echoserver client and another for our required webserver client. I did echoserver client as an additional task, though, not asked in the requirement

8. For the given extra credit requirement for our webserver, handling GET request, I have read the connection type status from the config file and if that's keep-alive, I will initiate timeout period as per the config file. I have created a struct timer variable and I used select statement to check for whether any request is made by the client during the timeout period. If there's any activity within that timeout period, timer will be reset to original timeout value. If there's no activity for that period, the server will be timed-out and disconnected

9.I will run the echo telnet client to check for the connection status and I will echo the status on the server. If the connection-type is keep-alive, the server will allow next request while handling this telnet client request. If pipelining is not possible, running the telnet command, will make the connection-close immediately. By the checking the timeout connection status using select and by doing forking, I'm able to handle multiple requests

10.For my own pipelining test for the echoserver, I use 'select' command to check for any activity during the timeout period and if there's any activity, the timeout period will be re-adjusted. I echo whatever I type as as a request on the telnet terminal and server will keep track of any requests received and accordingly adjust the timeout period. If there's no activity, the server will be timed-out

11.I have attached a code file web_server.c, login.html(for HTTP POST), a configuration file ws_config alomg with the Readme.txt 

12.My program is compatibile with GCC compiler

13.I have created a makefile to compile my code

14.I thank Professor and TA's for helping me out in understanding several concepts

