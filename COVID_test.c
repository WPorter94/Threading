#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
//Binary semaphore.
sem_t connected_lock;
//Binary semaphore.
sem_t id_lock;
//Counting semaphore.
sem_t operators;
//Number of operators.
static int NUM_OPERATORS = 4;
//Number of lines.
static int NUM_LINES = 7;
//Number of currently connected callers.
static int connected = 0;
//Id value for each caller, gets incremented in function phonecall.
int next_id = 0;
//Function that is resonsible fotre the simulation of a call center.
void* phonecall(void*vargp);
//Main function, responsible for initilization and cleanup. Takes a 1 command line argument(an integer), and returns a integer.
int main(int argc, char **argv){
    //Check if user input nothing.
    if(argc == 1){
        printf("No input entered, exiting program. Please try again with a single valid integer...\n");
        return 1;
    }
    //Check if user input more than 1 argument
    if(argc > 2){
        printf("Multiple inputs found. Program will use the last input entered.\n");
    }
    int threadCheck = 0;
    //Initialize semaphores.
    sem_init(&connected_lock,0,1);
    sem_init(&operators,0,4);
    sem_init(&id_lock,0,1);
    //Initialize variable totalCallers to the last command line argument passed by the user.
    //If argument passed is a character it will be run as 0.
    int totalCallers = atoi(argv[argc-1]);
    //Initialize an array of threads with length of totalCallers.
    pthread_t threadArr[totalCallers];
    //Create all threads and runs them through function phonecall.
    for(int i = 0; i < totalCallers; ++i){
        threadCheck = pthread_create(&threadArr[i],NULL, phonecall,NULL);
        while(threadCheck != 0){
            sleep(1);
            threadCheck = pthread_create(&threadArr[i],NULL, phonecall,NULL);
        }
    }
    //Waits for threads execute and then destroys all threads.
    for(int j = 0; j < totalCallers; ++j){
        pthread_join(threadArr[j], NULL);
    }
    //Destroy both semaphores.
    sem_destroy(&connected_lock);
    sem_destroy(&operators);
    sem_destroy(&id_lock);
    //End program.
    return 0;
}
//phonecall is a simulation of a working call center, with lines, operators, and callers.
//It takes created threads and returns nothing. 
void* phonecall(void* vargp){
    //Variable id is used to keep track of individual threads. It uses the global variable next_id to avoid repeating I.D numbers.
    
    sem_wait(&id_lock);
    int id = ++next_id;
    sem_post(&id_lock);
    printf("Caller #%d attempting to connect to call center...\n",id);
    //This loop simulates a caller trying to connect to an open line.
    while(1){ 
        //connected_lock binary semaphore only allows 1 caller through at a time.      
        sem_wait(&connected_lock);
        //all lines are in use.
        if(connected == NUM_LINES){
            //caller clears the binary semaphore and another can enter.
            sem_post(&connected_lock);
            printf("sorry caller #%d The line is busy...\n", id);
            //caller returns to beginning of loop.
            sleep(1);
        //there is at least one open line
        }else{
            //the calller has been connected
            ++connected;
            //caller clears the binary semaphore and another can enter.
            sem_post(&connected_lock);
            printf("Caller #%d has connected and is wating for an operator.\n", id);
            //caller clears the loop.
            break;
        }       
    }
    //Caller waits for operators to open for another thread. once this has happened they become connected to an operator and
    //enter the counting semaphore.
    //Please ignore the printf statement as it is premature and should occur in line 99.
    printf("Caller #%d is speaking to an operator.\n",id);
    sem_wait(&operators);
    sleep(3);
    //caller exits the counting semaphore so another can enter.
    sem_post(&operators); 
    printf("Caller #%d has made an appointment. An operator is available.\n",id);
    //thread needs to decrement connected variable which is in a forbidden region so the binary semaphore must be used again.
    sem_wait(&connected_lock);
    --connected;
    sem_post(&connected_lock);

    printf("Caller #%d has ended the call.\n",id);
}