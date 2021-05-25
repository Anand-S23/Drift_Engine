#ifndef DRIFT_DATA_STRUCTURES_H
#define DRIFT_DATA_STRUCTURES_H

typedef struct node
{
    struct node *next;
    void *data;
} node;

typedef struct linked_list
{
    node *head;
    node *tail;
    int lenght;
} linked_list;

typedef stack linked_list;
typedef queue linked_list;

#endif
