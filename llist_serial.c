/*  serial linkedlist - 2017
    cse 13 
    authors: 130217B, 130147J
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "timer.h"
#include "print.h"

struct Node
{
    int data;
    struct Node *next;
};

int Member(int value, struct Node *head_p);
int Insert(int value, struct Node **head_pp);
int Delete(int value, struct Node **head_pp);
void freeMemory(struct Node **head_pp);
void printList(struct Node *head_p);

int n, m;
float mMember, mInsert, mDelete;
double sum, sqrSum;
int sampleCount, count;

int main(int argc, char *argv[])
{

    struct Node *head = NULL;
    int memberCount = 0, insertCount = 0, deleteCount = 0;
    double sum = 0, sqrSum = 0;
    int count = 0;
    double startTime, finishTime, elapsedTime;

    if (argc != 7)
    {
        printf("Command required: ./llist_serial n m member insert delete samples\n");
    }
    n = (int)strtol(argv[1], (char **)NULL, 10);
    m = (int)strtol(argv[2], (char **)NULL, 10);

    mMember = (float)atof(argv[3]);
    mInsert = (float)atof(argv[4]);
    mDelete = (float)atof(argv[5]);
    sampleCount = (int)atof(argv[6]);

    if (n <= 0 || m <= 0 || mMember + mInsert + mDelete != 1.0)
    {
        printf("Command required: ./llist_serial n m member insert delete samples\n");
    }

    //printList(head);

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

        GET_TIME(startTime);
        int k;
        for (k = 0; k < m; k++)
        {
            float p = (rand() % 10000 / 10000.0);
            int r = rand() % 65536;

            if (p < mMember)
            {
                Member(r, head);
                memberCount++;
            }
            else if (p < mMember + mInsert)
            {
                Insert(r, &head);
                insertCount++;
            }
            else
            {
                Delete(r, &head);
                deleteCount++;
            }
            count += 1;
        }

        GET_TIME(finishTime);

        elapsedTime = finishTime - startTime;

        freeMemory(&head);
        sum += elapsedTime;
        sqrSum += elapsedTime * elapsedTime;

        // printf("Elapsed time: %f\n", elapsedTime);
    }

    printData("llserial", n, m, sampleCount, mMember, mInsert, mDelete, 1, sum, sqrSum);
    // printf("count :%i\n", memberCount);
    return 0;
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

void printList(struct Node *head_p)
{
    struct Node *current = head_p;

    while (current != NULL)
    {
        printf("printing data in node:%i\n", current->data);
        current = current->next;
    }
}