/*
 * unused.c
 * 
 * This file is used to test certain functions during development.
 */

#include "bm_list.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct list_node {
    char *data;
    struct list_node *next;
} Node;


int main(int argc, char **argv)
{
    Node *head = NULL;
    
    // put strings into list backwards
    while (--argc > 0) {
        Node *node = list_node();
        node->data = argv[argc];
        head = list_push(head, node);
    }

    // print out input
    while (!list_empty(head)) {
        char *data = list_remove(&head);
        printf("%s ", data);
    }
    putchar('\n');

    return 0;
}

