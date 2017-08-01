Readme instructions
1.pages_from_server - PASS
2.pages_from_proxy  - PASS
3.pages_from_server_after_cache_timeout - PASS
4.prefetching       - PASS, I guess it works for all websites, it might work for all websites as I have tested for couple of websites
5.multithreading  - PASS
6.connection keepalive  -PASS 

Youtube link where I uploaded my video. My code can't be tested with the script, because of the way I have written. But I have attached the video and uploaded in youtube. Link below
https://youtu.be/VUhV9V96iks


I have completed the proxy with caching, link prefetching, keep-alive and multi-threading.
I have submitted three code files. I apologize for that as I want to show all my work in creating new ways to implement this task

1.proxy_no_caching.c - it does general proxy, receiving and sending files from server and proxy and also used forking to handle multiple simultaneos connections from several clients
                       and also checks for keep-alive connections
2. proxy_cached_timeout.c - it does caching and link-prefetching along with above keep-alive and multi-threading. This program gets a user input for timeout. I have used threads here
3. proxy_cached_default_timeout.c- This program does the same as above, but it has a hard-coded timeout. So once again I apologize


How I used proxy?
1. I have tested on Mozila Firefox and I have tested on certain ports for the proxy in the preferences->advanced->network settings
2. Proxy acts as a intermediate between the client and the server in data transfer.
3. Proxy settings works for http websites and I have handled only GET and not POST method
4. I have used linked-list to store cached data and I have written lots of error handling functions before I implemented my caching and link pre-fetching.
That took more time for me to complete and so I had to stick with three different codes
5. I run a separate thread for caching and link-prefetching and so it will avoid any delays in fetching and sending data to the client
6. keepalive connection is addresses in about:config network proxy settings page by enabling that option
7.I have used both forking and pthreads for handling multiple-simultaneous connections


I thank both the TA's and the Professor for their support in helping me complete this assignment.

I would be happy to get feedback from TA's, if any of my functionalities don't work as expected and if I have wrongly stated as successful. I believe, I have addressed all the requirements.
But would be happy to listen to any comments about faults in my code. I apologize for any in-covenience and I appreciate your patience in reading my document

I'm again attaching my Youtube URL
https://youtu.be/VUhV9V96iks
 

The audio wasn't good. But please bear with me as I had to use a screen record app on windows.

Thank you




