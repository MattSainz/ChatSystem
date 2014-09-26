/* 
 * File:   dict.h
 * Author: http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html
 *
 * Created on September 22, 2014, 12:57 AM
 */

#ifndef DICT_H
#define	DICT_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct dict *Dict;
    
    /* create a new empty dictionary */
    Dict DictCreate(void);
    
   /* destroy a dictionary */
   void DictDestroy(Dict);
    
    /* insert a new key-value pair into an existing dictionary */
   void DictInsert(Dict, const char *key, const int value);
  
   /* return the most recently inserted value associated with a key */
   /* or 0 if no matching key is present */
   const int DictSearch(Dict, const char *key);
   
   /* delete the most recently inserted record with the given key */
   /* if there is no such record, has no effect */
   void DictDelete(Dict, const char *key);


#ifdef	__cplusplus
}
#endif

#endif	/* DICT_H */

