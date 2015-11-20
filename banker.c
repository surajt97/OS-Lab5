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





// Define functions declared in banker.h here


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

 bool request_res(int n_customer, int request[])
{

    for (int j = 0; j<NUM_CUSTOMERS; j++) {
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

// Release resources, returns true if successful
bool release_res(int n_customer, int release[])
{
    return true;
}



int main(int argc, char *argv[])
{
    
    // ==================== YOUR CODE HERE ==================== //

    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments   
    for (int i =0; i < NUM_RESOURCES; i++) {
        //Specifies the max INITIAL availale resources. MAX cannot exceed this
        available[i] = atoi(argv[i+1]);
        
        for(int j=0; j<NUM_CUSTOMERS; j++) {
            srand(time(NULL));
            maximum[j][i] = rand() % available[i];
            allocation[j][i] = rand()% available[i];
            printf("%d ", maximum[j][i]);
            need[j][i] = maximum[j][i] - allocation[j][i];

        }        
        printf("\n");

    }
    
    

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
