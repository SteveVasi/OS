# Semaphore_Training
C-Training-Example

This is a small exercise to train your skills for the bs-programming Test.
To ensure you have the skills to eventually pass the test with enough points
to at least graduate in this course.

The task is simple and just focus on the basics about the first test.

Task:

In general there are 2 programs. One server and one client with simple tasks.
Both of them should be able to communicate over semaphores and shared memory.

1: Client
The client opens a file and read it line by line. Each line is written in the
shared memory to pass it to the server. After the connection the client eliminates
it self. KEEP IN MIND THAT AFTER A CLIENT IS FINISHED ANOTHER SHOULD BE ABLE TO 
USE THE SAME SEMAPHORES (The client has to be startable several times).

The client can take some arguments with it. At least required is the file to read
from.
You can also give the client -u or -l which force the read line from the file 
to be written in the shared memory in upper/lower case. 

2: Server
The server just opens the shared memory and the semaphores if not already exist and
wait for a client to send. (Use the semaphore to perform that)
The server just reads from the shared memory after the client passes something in
and prints it to the stdout.

The server is waiting for a client connection. In case of a signal the server should be
able to handle it. This means the input CTRL + C does interrupt the server and ends the
program gracefully. 

The actual solution, which is working, is available --> Just look up.
You can try it your self with my code and inspect it. 
MARK --> I AM NOT TAKING RESPONSE OF ANY INCONVINIENCE OR TROUBLE ON YOUR
SYSTEM IF YOU USE MY CODE!
