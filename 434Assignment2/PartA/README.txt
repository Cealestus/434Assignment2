Matthew Eisler
mge987
11145250
CMPT 434 Assignment #2

To Run Part A, Question 1:

To run part A, the receiver should be started first, then the sender.
The makefile should first be ran to compile both files. This will produce a file labelled "sender" and a file labelled "receiver".
At the current version of the software, this will produce a warning about an unused TIMEOUT variable, which is known and does not
affect the performance of the system.

To start the receiver, simply type, "./receiver" into the terminal. This will begin the receiver system, and a prompt should appear,
stating: "listener: waiting to recvfrom..."

The starter should then be started. This requires the most amount of user input, requiring 5 statements. The format is as follows:
"./sender <IP Address of server> 30001 <Maximum number of frames> <Timeout value>". As an example, the input I give the program is:
"./sender 128.233.236.68 30001 10 10". The sender will not prompt for input, but will wait for text on the terminal.

At this point both systems have been started. Typing into the sender side should create indications on the sender and receiver that
data has been transferred between the two. Both sender and  receiver will produce textual feedback, but do not currently communicate
properly between eachother. The receiver currently requests the user to confirm whether or not the listed output was correctly
received.

Entering "exit" as input on the sender side will cause the sender to quit. The receiver must be manually exited by using "Ctrl+C"
to quit the application.

To Run Part A, Question 2:

I did not complete part 2, as I was having difficulty getting Question 1's components to use feedback for proper operation.