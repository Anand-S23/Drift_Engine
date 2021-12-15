#include "drift_data_structures.h"

/* Linked List */

// Makes a new node using malloc
internal node *CreateNode(void *data, node* next)
{
    node *ret_node = (node *)malloc(sizeof(node));
    ret_node->data = data;
    ret_node->next = next;
    ret_node->prev = NULL;
    return ret_node;
}

// Makes a new empty list
internal linked_list CreateList()
{
    linked_list list = {0};
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    return list; 
} 

// Pushes node on the beginning of the list
internal void Push(linked_list *list, void *data)
{
    node *new_node = CreateNode(data, list->head);
    list->head->prev = new_node;
    list->head = new_node;

    if (list->tail == NULL)
    {
        list->tail = list->head->next;
    }

    ++list->length;
}

#define PushType(list, type, data)               \
{                                                \
    type x = data;                               \
    Push(list, &x);                              \
}                                                \

// Appends node to the end of the list
internal void Append(linked_list *list, void *data)
{
    node *new_node = CreateNode(data, NULL);

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else if (list->tail == NULL)
    {
        list->tail = new_node;
        list->head->next = list->tail;
        list->tail->prev = list->head;
    }
    else
    {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }

    ++list->length;
}

#define AppendType(list, type, data)             \
{                                                \
    type x = data;                               \
    Append(list, &x);                            \
}                                                \

// Inserts a node into specified position
internal b32 Insert(linked_list *list, void *data, int pos)
{
    if (pos > list->length || pos < 0)
    {
        return 0;
    }

    node *current = list->head;
    for (int i = 0; i < pos - 1; ++i)
    {
        current = current->next;
    }

    node *new_node = CreateNode(data, NULL);
    new_node->next = current->next;
    new_node->prev = current;
    current->next = new_node;

    ++list->length;
    return 1;
}

#define InsertType(list, type, data, pos)        \
{                                                \
    type x = data;                               \
    Insert(list, &x, pos);                       \
}                                                \

// Swaps two nodes at and returns if it was sucessful
internal b32 Swap(linked_list *list, int pos1, int pos2)
{
    if (pos1 < 0 || pos1 >= list->length ||
        pos2 < 0 || pos2 >= list->length ||
        pos1 == pos2)
    {
        return 0;
    }

    node *current1 = list->head;
    for (int i = 0; i < pos1; ++i)
    {
        current1 = current1->next;
    }

    node *current2 = list->head;
    for (int i = 0; i < pos2; ++i)
    {
        current2 = current2->next;
    }

    node *temp_pre1 = current1->prev;
    node *temp_post1 = current1->next;
    node *temp_pre2 = current2->prev;
    node *temp_post2 = current2->next;

    current2->prev = temp_pre1;
    if (temp_pre1 != NULL)
    {
        temp_pre1->next = current2;
    }
    current2->next = temp_post1;
    if (temp_post1 != NULL)
    {
        temp_post1->prev = current2;
    }

    current1->prev = temp_pre2;
    if (temp_pre2 != NULL)
    {
        temp_pre2->next = current1;
    }
    current1->next = temp_post2;
    if (temp_post2 != NULL)
    {
        temp_post2->prev = current1;
    }

    return 1;
}

// Removes the first element from the list
internal void *Pop(linked_list *list)
{
    void *data = list->head->data;
    node *temp = list->head;
    list->head = list->head->next;
    list->head->prev = NULL;
    free(temp);

    if (list->tail != NULL)
    {
        if (list->head->next == NULL)
        {
            list->tail = NULL;
        }
    }

    --list->length;
    return data;
}

#define PopType(list, type) ((type *)Pop(list, pos))

// Removes the last element from the list
internal void *PopLast(linked_list *list)
{
    node *current = list->head;
    for (int i = 0; i < list->length - 2; ++i)
    {
        current = current->next;
    }

    void *data = list->tail->data;
    node *temp = current->next;
    list->tail = current;
    list->tail->next = NULL;
    free(temp);

    return data;
}

#define PopLastType(list, type) ((type *)PopLast(list, pos))

// Removes element form position
internal void *Remove(linked_list *list, int pos)
{
    if (pos >= list->length || pos < 0)
    {
        return NULL;
    }

    node *current = list->head;
    for (int i = 0; i < pos - 1; ++i)
    {
        current = current->next;
    }

    if (current->next == NULL)
    {
        return NULL;
    }

    node *temp = current->next;
    void *data = temp->data;
    current->next = current->next->next;
    current->next->prev = current;
    free(temp);

    return data;
}

#define RemoveType(list, pos, type) ((type *)Remove(list, pos))
    
// Returns the data of the top node
internal void *Top(linked_list *list)
{
    return list->head->data;
}

#define TopType(list, type) ((type *)Top(list))

// Returns the data of the last node
internal void *Bottom(linked_list *list)
{
    return list->tail->data;
}

#define BottomType(list, type) ((type *)Bottom(list))

// Returns the data of node at index passed in
internal void *Get(linked_list *list, int pos)
{
    if (pos < 0 || pos >= list->length)
    {
        return NULL;
    }

    node *current = list->head;
    for (int i = 0; i < pos; ++i)
    {
        current = current->next;
    }

    return current->data;
}

#define GetType(list, pos, type) ((type *)Get(list, pos))

// Checks if the list is empty
internal b32 Empty(linked_list *list)
{
    return list->head == NULL;
}
