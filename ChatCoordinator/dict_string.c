#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dict_string.h"

struct elt 
{
  struct elt *next;
  int key;
  char *value;
};

struct dict 
{
  int size;           /* size of the pointer table */
  int n;              /* number of elements stored */
  struct elt **table;
};

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

/* dictionary initialization code used in both DictCreate and grow */
Dict
internalDictCreateS(int size)
{
    Dict d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);

    for(i = 0; i < d->size; i++) d->table[i] = 0;

    return d;
}

Dict
DictCreateS(void)
{
    return internalDictCreateS(INITIAL_SIZE);
}

void
DictDestroyS(Dict d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long
hash_functionS(int index)
{
    return index;
}

static void
growS(Dict d)
{
  Dict d2;            /* new dictionary we'll create */
  struct dict swap;   /* temporary structure for brain transplant */
  int i;
  struct elt *e;

  d2 = internalDictCreateS(d->size * GROWTH_FACTOR);

  for(i = 0; i < d->size; i++) {
      for(e = d->table[i]; e != 0; e = e->next) {
          /* note: this recopies everything */
          /* a more efficient implementation would
           * patch out the strdups inside DictInsert
           * to avoid this problem */
          DictInsertS(d2, e->key, e->value);
      }
  }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictDestroy on d2 */
  swap = *d;
  *d = *d2;
  *d2 = swap;

  DictDestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void
DictInsertS(Dict d, const int key, const char *value)
{
    struct elt *e;
    unsigned long h;

    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = key;
    e->value = strdup(value);

    h = hash_functionS(key);

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        growS(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char *
DictSearchS(Dict d, const int key)
{
    struct elt *e;

    for(e = d->table[hash_functionS(key) % d->size]; e != 0; e = e->next) {
        if(e->key == key) {
            /* got it */
            return e->value;
        }
    }

    return 0;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void
DictDeleteS(Dict d, const int key)
{
    struct elt **prev;          /* what to change when elt is deleted */
    struct elt *e;              /* what to delete */

    for(prev = &(d->table[hash_functionS(key) % d->size]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if((*prev)->key != key) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->value);
            free(e);

            return;
        }
    }
}
