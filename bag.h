#ifndef BAG_H_
#define BAG_H_
#include <stdio.h>
#include <stdbool.h>
#include "crawler.h"

typedef struct node node;
struct node {
  webpage_t *key;
  int* item;
  node *next;
  node *prev;
};

typedef struct set_t set_t;
struct set_t {
  int size; 
  node *head;
  node *tail;
};

/* Create a new (empty) set; return NULL if error. */
set_t* set_new(void);

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool set_insert(set_t *set, webpage_t *key, int *item);

/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, webpage_t *key);

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, webpage_t *key, int *item) );

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, webpage_t *key, int *item) );

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(int *item) );

#endif // BAG_H_
