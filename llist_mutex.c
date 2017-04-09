/*  mutex linkedlist - 2017
    cse 13 
    authors: 130217B, 130147J
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "timer.h"
#include "print.h"

#include <pthread.h>

struct Node
{
    int data;
    struct Node *next;
};

int Member(int value, struct Node *head_p);
int Insert(int value, struct Node **head_pp);
int Delete(int value, struct Node **head_pp);
void *Operations(void *rank);
void freeMemory(struct Node **head_pp);

int n, m, sampleCount;
float mMember, mInsert, mDelete;
double sum, sqrSum;

const int MAX_THREADS = 1024;
long thread_count;
pthread_mutex_t mutex;
int memberCount = 0, insertCount = 0, deleteCount = 0;
struct Node *head = NULL;

int main(int argc, char *argv[])
{

    double startTime, finishTime, elapsedTime;
    sum = 0, sqrSum = 0;

    long thread;
    pthread_t *thread_handles;

    if (argc != 8)
    {
        printf("Command required: ./llist_mutex numOfThreads n m member insert delete samples\n");
    }

    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS)
    {
        printf("Please give the command: ./llist_mutex numOfThreads n m member insert delete samples\n");
    }

    n = (int)strtol(argv[2], (char **)NULL, 10);
    m = (int)strtol(argv[3], (char **)NULL, 10);

    mMember = (float)atof(argv[4]);
    mInsert = (float)atof(argv[5]);
    mDelete = (float)atof(argv[6]);
    sampleCount = (float)atof(argv[7]);

    if (n <= 0 || m <= 0 || mMember + mInsert + mDelete != 1.0)
    {
        printf("Command required: ./llist_mutex numOfThreads n m member insert delete samples\n");
    }

    int j;
    for (j = 0; j < sampleCount; j++)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            int r = rand() % 65536;
            if (!Insert(r, &head))
            {
                i--;
            }
        }

        thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

        GET_TIME(startTime);

        pthread_mutex_init(&mutex, NULL);

        for (thread = 0; thread < thread_count; thread++)
        {
            pthread_create(&thread_handles[thread], NULL, Operations, (void *)thread);
        }

        for (thread = 0; thread < thread_count; thread++)
        {
            pthread_join(thread_handles[thread], NULL);
        }

        pthread_mutex_destroy(&mutex);

        GET_TIME(finishTime);

        elapsedTime = finishTime - startTime;
        freeMemory(&head);
        sum += elapsedTime;
        sqrSum += elapsedTime*elapsedTime;

        // printf("Elapsed time: %f\n", elapsedTime);
    }

    printData("mutex", n, m, sampleCount, mMember, mInsert, mDelete, thread_count, sum, sqrSum);
    // printf("count :%i\n", memberCount);

    return 0;
}

void *Operations(void *rank)
{
    long i;
    long fraction = m / thread_count;
    for (i = 0; i < fraction; i++)
    {
        float p = (rand() % 10000 / 10000.0);
        int r = rand() % 65536;
        // printf("%f\n", p);

        if (p < mMember)
        {
            pthread_mutex_lock(&mutex);
            Member(r, head);
            // printf("in member\n");
            memberCount++;
            pthread_mutex_unlock(&mutex);
        }
        else if (p < mMember + mInsert)
        {
            pthread_mutex_lock(&mutex);
            Insert(r, &head);
            // printf("in Insert\n");
            insertCount++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_lock(&mutex);
            Delete(r, &head);
            // printf("in Delete\n");
            deleteCount++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int Member(int value, struct Node *head_p)
{
    struct Node *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Insert(int value, struct Node **head_pp)
{
    struct Node *curr_p = *head_pp;
    struct Node *pred_p = NULL;
    struct Node *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct Node));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL)
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

int Delete(int value, struct Node **head_pp)
{

    struct Node *curr_p = *head_pp;
    struct Node *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value)
    {
        if (pred_p == NULL)
        {
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

void freeMemory(struct Node **head_pp)
{
    struct Node *curr_p;
    struct Node *succ_p;

    if (head_pp == NULL)
        return;

    curr_p = *head_pp;
    succ_p = curr_p->next;

    while (succ_p != NULL)
    {
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->next;
    }

    free(curr_p);
    *head_pp = NULL;
}