/*
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file trie.h
 * @author Ashutosh Grewal on 12/10/16.
 *
 * @brief Header file containing APIs to the trie data structure
 * and some common data structures definition and declarations.
 *
 * @attention
 * The forward declaration of trie only allows to create pointers
 * to this struct and not the struct itself while the definition of
 * boolean - as expected - doesn't put any restrictions.
 */
 

#ifndef _TRIE_H_
#define _TRIE_H_


/**
 * @brief Enum representing a boolean.
 */
typedef enum bool {
    FALSE,
    TRUE
}boolean;
typedef struct trie_s trie_t;

boolean add_to_trie (char *, int, trie_t *);
boolean delete_from_trie (trie_t *, char *);
boolean lookup_in_trie (trie_t *, char *, int *value);
trie_t *create_trie (void);
void destroy_trie (trie_t *);

#endif /* _TRIE_H_ */
