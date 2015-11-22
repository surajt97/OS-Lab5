/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <Nicholas Gregorio, Jason Runzer, Albert Fung>
 * All rights reserved.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
 #include <time.h> //Used for random function
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3


// Put global environment variables here
// Available amount of each resource
int available[NUM_RESOURCES];

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

pthread_mutex_t lock;




// Define functions declared in banker.h here
void *customerMethod(void *customerNum){
    int customer = *(int*)customerNum;
    bool acquiredResources = false;
    int request [NUM_RESOURCES];
    int release [NUM_RESOURCES];
    
    while(1){
        for(int i = 0; i < NUM_RESOURCES; i++){
            request[i] = rand() % (maximum[customer][i] + 1);
        }
        pthread_mutex_lock(&lock);
        acquiredResources = request_res(customer,request);
        pthread_mutex_unlock(&lock);
        if(acquiredResources){
            printf("Customer: %d got the resources it wanted! The customer says they will use them and give them back in a bit.\n\n", customer);
            sleep((int)rand() % 5 + 1);
            
            acquiredResources = false;
        }
        for(int i = 0; i < NUM_RESOURCES; i++){
            srand(time(NULL));
            release[i] = rand() % (allocation[customer][i] + 1);
        }
        pthread_mutex_lock(&lock);
        release_res(customer,release);
        pthread_mutex_unlock(&lock);
    }

}

bool is_safe() {
    //Create & initialize variables for checking the safe sequences
    int size = sizeof(available) / sizeof(available[0]);
    int work[size];
    bool finish[NUM_CUSTOMERS];
    int finishCounter=0;
    bool safe = true;


    //Copy the request array to the work array.
    for(int i = 0; i <= size; i++){
        work[i] = available[i];
        
    }

    //Initialize finish array
    for (int j = 0; j<NUM_CUSTOMERS; j++) {
        finish[j] = false;
    }

    /* Nested for loop for traversing the 2 dimensional arrays
    *if a given need element is less or equal to a corresponding work element, and if the current finishCounter is false, we want to increment the work value by the corresponding allocation element
    *Otherwise, we want to keep our given finish element at false and increment the counter
    *The final finish array will will determine the outcome of is_safe
    */
    for (int row = 0; row < NUM_RESOURCES; row++) {
        for (int column = 0; column < NUM_CUSTOMERS; column++) {
            if ((need[row][column] <= work[column]) && (finish[finishCounter] == false)) {
                work[column] = work[column] + allocation[row][column];
                finish[finishCounter] = true;
                finishCounter++;
            } else {
                finish[finishCounter] = false;
                finishCounter++;
                
            }

        }
    }


    for (int count2=0; count2<NUM_CUSTOMERS; count2++) {
        if (finish[count2] != true) {
            safe = false;
            break;
        }
    }
    return safe;

   
}

bool request_res(int n_customer, int request[]){
   
    printf("Customer: %d Requesting Resources\n",n_customer);
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ",request[i]);
    }

    puts("");
     printf("Available Resources Before\n");
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ",available[i]);
    }
    puts("");
     for(int i = 0; i < NUM_RESOURCES; i++){
        if(request[i] <= need[n_customer][i]){
            if(request[i] > available[i]){
                printf("THIS WOULD LEAD TO AN UNSAFE STATE! ABORT ABORT! ALL HANDS ON DECK! FUCK WE GONNA DIE!!!!\n");
                sleep(2);
                return false;
            }
        } else{
            printf("Customer: %d is WAY TOO NEEDY\n",n_customer);
            sleep(2);
            return false;
        }

    }
    printf("Customer: %d Taking Resources\n",n_customer);
    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] -= request[i];
        need[n_customer][i] -= request[i];
        allocation[n_customer][i] += request[i];
    }
    printf("Available Resources After\n");
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ",available[i]);
    }
    puts("");
    return true;
}
/*
 bool request_res(int n_customer, int request[])
{

    for (int j = 0; j<NUM_RESOURCES; j++) {
        if (request[j] <= need[n_customer][j]) {
            if (request[j] <= available[j]) {
                printf("Request valid so far");
                available[j] = available[j] - request[j];
                allocation[n_customer][j] = allocation[n_customer][j] + request[j];
                need[n_customer][j] = need[n_customer] - request[j];

                if (is_safe()) {
                    //Do nothing; allocation is safe
                } else {
                    //Return resources
                }

            } else {
                //Wait; resources unavailable
            }
        } else {
            //raise error condition, exceeded max amount
        }
    }

    return true;
}

*/
// Release resources, returns true if successful
bool release_res(int n_customer, int release[])
{   
    puts("");
    printf("Customer: %d Releasing Resources\n", n_customer);
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ",release[i]);
        available[i] += release[i];
        need[n_customer][i] += release[i];
        allocation[n_customer][i] -= release[i];
    }
    puts("");
    printf("Available Resources\n");
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ",available[i]);
    }
    puts("");
    return true;
}



int main(int argc, char *argv[])
{   
    
    // ==================== YOUR CODE HERE ==================== //

    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments   
    pthread_t threads[NUM_CUSTOMERS];
    pthread_mutex_init(&lock, NULL);
    for (int i =0; i < NUM_RESOURCES; i++) {
        //Specifies the max INITIAL availale resources. MAX cannot exceed this
        available[i] = atoi(argv[i+1]);
        
        for(int j=0; j<NUM_CUSTOMERS; j++) {
            //srand(time(NULL));
            
            maximum[j][i] = rand() % (available[i] + 1);
            //allocation[j][i] = rand()% available[i];
            //printf("%d ", maximum[j][i]);
            need[j][i] = maximum[j][i];
        }        
    }

    printf("Maximum Needs\n");
    for(int i = 0; i < NUM_CUSTOMERS; i++){
        for(int j = 0; j < NUM_RESOURCES; j++){
            printf("%3d ",maximum[i][j]);
        }
        puts("");
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        int *c_num = malloc(sizeof(*c_num));
        if(c_num ==NULL){
            printf("couldn't make customer number");
            exit(1);
        }
        *c_num = i;
        pthread_create(&threads[i],NULL,&customerMethod, c_num);
    }
    
    //join all the threads
    for(int i = 0; i < NUM_CUSTOMERS; i++){
        pthread_join(threads[i],0);
    }
    pthread_mutex_destroy(&lock);
    //int max_resources [NUM_RESOURCES];

    printf("Made to check safe\n");
    
    if(is_safe()) {
        printf("System is safe\n");
    } else {
        printf("System is not safe\n");
    }
    
    // Allocate the available resources
    //if request res is true, allocate


    // Initialize the pthreads, locks, mutexes, etc.

    // Run the threads and continually loop

    // The threads will request and then release random numbers of resources

    // If your program hangs you may have a deadlock, otherwise you *may* have
    // implemented the banker's algorithm correctly
    
    // If you are having issues try and limit the number of threads (NUM_CUSTOMERS)
    // to just 2 and focus on getting the multithreading working for just two threads

    return EXIT_SUCCESS;
}
