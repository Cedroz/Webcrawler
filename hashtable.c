#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "bag.h"

unsigned int hash(const char *key, int maxSlots) {
  int len = strlen(key);
  unsigned int hash_val = 0;
  for (int i = 0; i < len; i++) {
    hash_val += key[i];
  }
  return hash_val%maxSlots;
}



hashtable_t *hashtable_new(const int num_slots) {
  hashtable_t* hashtable = (hashtable_t *) malloc(sizeof(hashtable_t)); //Allocates space
  hashtable->items = (nodeH**) malloc(num_slots*sizeof(nodeH*));
  hashtable->size = num_slots;
  if (hashtable == NULL) { //Checks if error occurs
      return NULL;
  }
  for (int i = 0; i < num_slots; i++) { //Sets all items in the hashtable to NULL
      hashtable->items[i] = NULL;
  }
  return hashtable; 
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
  int slot = hash(key,ht->size);
  nodeH *newNode = (nodeH *)malloc(sizeof(nodeH));
  if (newNode==NULL || ht==NULL || key == NULL || item == NULL) {
    return false;
  }
  for (int i = 0; i < ht->size; i++) {
    nodeH *current = ht->items[i];
    while (current != NULL) {
      if (strcmp(current->key,key)==0) {
        return false;
      }
      current=current->next;
    }
  }
  newNode->key = strdup(key);
  newNode->value = item;
  newNode->next = ht->items[slot];
  ht->items[slot] = newNode;
  return true;
}

void *hashtable_find(hashtable_t *ht, const char *key) {
  int slot = hash(key,ht->size);
  if (ht == NULL || key == NULL) {
      return NULL;
    }
    nodeH *currentNode = ht->items[slot];
    while (currentNode != NULL && currentNode->key != NULL) {
      if (strcmp(currentNode -> key, key) == 0) {
        return currentNode->value;
      }
      currentNode = currentNode -> next; 
    }
    return NULL;
  }

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) { //Frees memory
    if (ht!=NULL) {
        nodeH *nextNode;
        for (int i = 0; i < ht->size; i++) {
          nodeH* currentNode = ht->items[i];
          while (currentNode != NULL) {
            nextNode = currentNode->next;
            if (itemdelete != NULL) {
                itemdelete(currentNode->key);
                itemdelete(currentNode->value);
            } 
            free(currentNode);
            currentNode = nextNode;
        }
    }
  free(ht->items); 
  free(ht);
  }
}
