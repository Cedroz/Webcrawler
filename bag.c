#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bag.h"
#include "crawler.h"

set_t* set_new(void) {
  set_t* set = (set_t *) malloc(sizeof(set_t)); // Allocates space for set_t
  if (set == NULL) { // Checks if error occurs
    return NULL;
  }

  set->size = 0;  // Initialize the size
  set->head = NULL; // Initializes the head of the linked list
  set->tail = NULL; // Initializes the tail of the linked list

  return set;
}


bool set_insert(set_t *set, webpage_t *key, int *item) {
  node* newNode = (node *) malloc(sizeof(node)); //Allocates space
  if (key == NULL) { //Checks if key or Item is null
    return false;
  }

  newNode->key = key; //Gives new node the key
  newNode->item = item; //Gives new node the item
  newNode->next = NULL; //The new node will be at the end, so its next should be NULL
  newNode->prev = NULL; //Initialize prev pointer

  if (set->head == NULL) { //If the set is NULL, the new node is both the head and the tail
    set->head = newNode;
    set->tail = newNode;
  } 
  else { //Traverses to the end of the set to find the current tail
    node* tail = set->head;
    while (tail->next != NULL) {
      tail = tail->next;
  }

    tail->next = newNode; //Insert the new node at the end
    newNode->prev = tail;
    set->tail = newNode; //Update tail to newNode
  }

  return true;
}

void *set_find(set_t *set, webpage_t *key) { //Uses the key to return the associated item, returns null if set is null, key is null or if the key is not found
  if (set == NULL || key == NULL) {
    return NULL;
  }
  node *currentNode = set->head;
  while (currentNode != NULL && currentNode->key != NULL) {
    if (strcmp(currentNode->key->url, key->url) == 0) {
      return currentNode->item;
    }
    currentNode = currentNode -> next; 
  }
  return NULL;
}


void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, webpage_t *key, int *item) ) { //prints set into a given file if the given function and set is not null
  if (fp != NULL) {
    if (set == NULL || set->head == NULL || itemprint == NULL) {
      printf("(null)");
    }
    else {
      node* currentNode = set->head;
      while (currentNode != NULL) {
        itemprint(fp, currentNode->key, currentNode->item);		
        currentNode = currentNode -> next;  
      }
    }
  }
}

void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, webpage_t *key, int *item) ) { //prints set into a given file if the given function and set is not null
  if (set != NULL && itemfunc != NULL) {
    node* currentNode = set->head;
    while (currentNode != NULL) {
        itemfunc(arg, currentNode->key, currentNode->item);		
        currentNode = currentNode -> next;  
      }
  }
}

void set_delete(set_t *set, void (*itemdelete)(int *item)) { //Frees memory
    if (set != NULL) {
        node *currentNode = set->head;
        node *nextNode;
        while (currentNode != NULL) {
            nextNode = currentNode->next;
            if (itemdelete != NULL) {
                free(currentNode->key);
                free(currentNode->item);
            } 
            free(currentNode);
            currentNode = nextNode;
        }
        set->head = NULL;
    }
}
