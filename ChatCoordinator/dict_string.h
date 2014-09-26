/* 
 * File:   dict_string.h
 * Author: user
 *
 * Created on September 26, 2014, 12:06 AM
 */

#ifndef DICT_STRING_H
#define	DICT_STRING_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct dict *Dict;
    
/* create a new empty dictionary */
Dict DictCreateS(void);
    
/* destroy a dictionary */
void DictDestroyS(Dict);
    
/* insert a new key-value pair into an existing dictionary */
void DictInsertS(Dict, const int key, const char *value);
  
/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char* DictSearchS(Dict, const int key);
   
/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDeleteS(Dict, const int key);

#ifdef	__cplusplus
}
#endif

#endif	/* DICT_STRING_H */
