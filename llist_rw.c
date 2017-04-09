/*  rw linkedlist - 2017
    cse 13 
    authors: 130217B, 130147J
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"
#include "print.h"
 
// Node 
struct Node
{
    int data;
    struct Node* next;
};
 
const int MAX_THREADS = 1024;
 
long thread_count;
pthread_rwlock_t rwlock;
struct Node* head = NULL;    
 
int n;
int m;
int samples;
float mMember;
float mInsert;
float mDelete;
int count_member=0;
int count_insert=0;
int count_delete=0;
 
int Member( int value, struct  Node* head_p );
int Insert(int value, struct Node** head_pp);
int Delete (int value, struct Node** head_pp);
void* Operations(void* rank);
void getInputs(int argc, char* argv[]);
void freeMemory(struct Node** head_pp);
void fillLinkedList(int n,  struct Node** head_pp);
 
/* Main function */
int main(int argc, char* argv[])
{
    long thread;
    pthread_t* thread_handles;
    double start, finish, elapsedTime;
     
    getInputs(argc, argv); 

    double sum = 0, squareSum = 0;
    int s;
    for(s=0; s<samples; s++)
    {
        fillLinkedList(n,&head); 
        thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));  
        GET_TIME(start);
        pthread_rwlock_init(&rwlock, NULL);
        
        for (thread = 0; thread < thread_count; thread++) {
            pthread_create(&thread_handles[thread], NULL,Operations , (void*)thread);  
        }
        
        for (thread = 0; thread < thread_count; thread++) {
            pthread_join(thread_handles[thread], NULL); 
        }
        
        pthread_rwlock_destroy(&rwlock);
        GET_TIME(finish);

        // Free the memory
        freeMemory(&head);
        elapsedTime = finish - start;
        sum+= elapsedTime;
        squareSum += elapsedTime*elapsedTime;
        // printf("Elapsed time = %f\n", elapsed);
    }

    printData("rwLock",n,m, samples,mMember, mInsert, mDelete, thread_count,sum, squareSum);
    return 0;
}

void fillLinkedList(int n,  struct Node** head_pp){
    	int i;
        for(i=0; i<n; i++)
    	{
    		int r = rand()%65536;
    	    if(!Insert(r,head_pp))
    	    {
    	    	i--;
    	    }
    	}
}

void getInputs(int argc, char* argv[]) {
    if (argc != 8)
    {
        printf("Please type the command: ./llist_rw numOfThreads n m mMember mInsert mDelete numOfSamples\n");
    }
    thread_count = strtol(argv[1], NULL, 10);  
    if (thread_count <= 0 || thread_count > MAX_THREADS)
    {
        printf("Please type the command: ./llist_rw numOfThreads n m mMember mInsert mDelete numOfSamples\n");
    }
    
    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);
     
    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);
    samples = (int) strtol(argv[7], (char **)NULL, 10);
     
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0)
   {
	printf("Please type the command: ./llist_rw numOfThreads n m mMember mInsert mDelete numOfSamples\n");
   } 
} 

void* Operations(void* rank) 
{
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i< my_m; i++ )
    {
        float prob = (rand()%10000/10000.0);    
     
        int r = rand()%65536;
        if(prob < mMember)
        {
            pthread_rwlock_rdlock(&rwlock);
            Member(r,head);
            count_member++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_rwlock_wrlock(&rwlock);
            Insert(r,&head);
            count_insert++;
            pthread_rwlock_unlock(&rwlock);
        }
        else
        {           
            pthread_rwlock_wrlock(&rwlock);
            Delete(r,&head);
            count_delete++;
            pthread_rwlock_unlock(&rwlock);
        }
    }
   return NULL;
} 

int Member( int value, struct  Node* head_p )
{
    struct Node* curr_p = head_p;
     
    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }
 
    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Insert(int value, struct Node** head_pp)
{
    struct Node* curr_p = *head_pp;          
    struct Node* pred_p = NULL;
    struct Node* temp_p = NULL;
 
    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
     
    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct Node));        
        temp_p->data = value;
        temp_p->next = curr_p;
         
        if(pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;
  
    }
    else
    {
        return 0;
    }
} 

int Delete (int value, struct Node** head_pp)
{
    struct Node* curr_p = *head_pp;
    struct Node* pred_p = NULL;
     
    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }   
     
    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
         
    }
    else
    {
        return 0;
    }
 
} 

void freeMemory(struct Node** head_pp) {
	struct Node* curr_p;
	struct Node* succ_p;

	if (head_pp == NULL)
		return;

	curr_p = *head_pp;
	succ_p = curr_p->next;

	while (succ_p != NULL) {
		free(curr_p);
		curr_p = succ_p;
		succ_p = curr_p->next;
	}

	free(curr_p);
	*head_pp = NULL;
}