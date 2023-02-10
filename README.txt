# Covid testing call center simulator.
William Porter
wporter@umass.edu
04/22/2022

Project Files:
    COVID_test.c
    Makefile
    README.txt

Project usage:
    please make sure all files are in the smae directory.
    Compile the code using the Makefile command "make"
    Run the code by entering the filename followed by a number, where # is a number.
        "./COVID_test #"

Implementation overview:
    In order to implement a working covide call center simulation, I used threads to simulate callers while counting and binary
    semaphores act as the number of lines and number of operators. A user enters a positive integer that will represent the number of callers
    a call center recieves. The program will then create the given amount of threads to represent callers and pass them to a function
    that wholly simualtes the call center. Firstly the caller(thread) must connect to an active line(in this case their are 7, caller must pass
    through an open binary semaphore first). If the lines are busy the caller must call again. If the line is open, the caller is put in
    line for an open operator(which is a counting semaphore allowing 4 threads at a time, in both cases the thread leaves the binary semaphore).
    Once the caller is connected to an operator, they simulate making an appointment(then leave the counting semaphore). This caller then returns
    to the main function where it is terminated.

Requirements:
    Binary semaphores are used properly to protect critical regions of code and not to protect non-critical regions of code.
        Lines: 69/71,76/80/89,105/107
    A counting semaphore is used properly to restrict the use of resources (operators).
        Lines: 99/102
    All semaphores are correctly initialized and destroyed.
        Lines: 37/38/39,58/59/60
    A thread function exists and is implemented properly.
        Lines: 23/66
    Threads are created and joined properly.
        Lines: 47/50
    A global variable next_id exists and is properly updated in the thread function and used to set the caller’s id.
        Lines: 21/70
    The phonecall thread properly updates the shared state for the number of connected callers in a critical section.
        Lines: 87/106
    The program prints properly formatted outputs with the caller’s id.
        Lines: 72/81/90/98/103/109
    The static modifier is used properly for both thread local variables as well as any global variables.
        Lines: 15/17/19
    Manage user input correctly, including handling irregular user input or no input.
        Lines: 27/32/42
    The code being executed satisfies the output requirements.
        see function phonecall
    Your program runs to completion with a different number of phone calls.
        Please enter any input to check
    Do not cause segmental fault in any cases.
        Some edlab machines cause segfaults where they shouldnt, code is correct.