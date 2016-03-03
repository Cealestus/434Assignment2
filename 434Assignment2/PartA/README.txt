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
"./sender 128.233.236.68 30001 10 10". The sender will then prompt 