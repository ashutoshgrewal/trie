/*
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file trie.c
 * @brief This file implements the trie data structure.
 * @details
 * The trie data structure is implemented as a node with 26 (english alphabet size) children.
 * Each child's key is the integer value of 26 letters. The tree starts down from the root.
 * The first level of the tree contains the first character of each of the keys (if there
 * are multiple keys with the same starting character, they'll share this element), the
 * second level contains the second character of the keys and so forth. While adding the last
 * character of the key, we mark that this element has a value and place the value in the
 * element. Deletion requires that we delete each element that leads us to the element with value.
 *
 * @author Ashutosh Grewal on 12/10/16.
 *
 * @bug No bugs are know at this point.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trie.h"

#define NUM_CHILD 26

/**
 * @brief An individual element of the trie.
 *
 * @details
 * A node with in each level of the trie that contains a character from the key 
 * and points to the next level of the trie. If the element contains a value, it
 * is so marked and the value stored.
 */
typedef struct node_s {
    struct node_s *child[NUM_CHILD];  /**< Pointers to the next level of trie. */
    char key;                         /**< Character of the key at this level. */
    int value;                        /**< Value stored for a particular key. */
    boolean has_value;                /**< Boolean indicating if a value is stored or
                                           this node has no value and is just part of
                                           the chain to reach the next level. */
} node_t;

/**
 * @brief Trie data structure.
 *
 * @details
 * A trie data structure contains multiple levels. The first level contains 26
 * elements to accomodate all 26 possible english alphabet characters. Each of
 * these 26 characters can point to 26 elements at the second level to represent
 * all possible second characters of keys.
 */
struct trie_s {
    node_t *child;                     /**< Pointer to the node that will point level
                                        of the trie. */
};

/*
 * Forward declarations.
 */
static boolean key_permitted (char *);
static unsigned char key_to_index (char);
static boolean node_has_multiple_children (node_t *);
static boolean node_has_children (node_t *node);

/**
 * @brief Create the trie data structure.
 *
 * @details.
 * Allocate the memory for trie and the child node that 
 * point to level 1.
 *
 * @return Pointer to trie or NULL if memory allocation failed.
 */
trie_t *create_trie (void)
{
    trie_t *trie;
    
    trie = (trie_t *) malloc (sizeof(trie_t));
    
    if (trie) {
        trie->child = (node_t *)malloc(sizeof(node_t));
        if (!trie->child) {
            free(trie);
            
            return NULL;
        }
        memset(trie->child, 0, sizeof(node_t));
    }
    
    return trie;
}

/**
 * @brief Add a value with a particular key.
 *
 * @details
 * Create chains or reuse ones that already exist to put elements
 * with all key characters at subsequent levels in the trie data
 * structure.
 *
 * @param[in] key The key provided to us.
 * @param[in] value Value corresponding to the key.
 * @param[in] trie Pointer to the trie.
 *
 * @return Boolean indicating if we succeeded or not.
 */
boolean add_to_trie (char *key, int value, trie_t *trie)
{
    if ((trie == NULL) || !key_permitted(key)) {
        return FALSE;
    }
    
    unsigned int size_of_key;
    
    size_of_key = strlen(key);
    node_t *node;
    
    node = trie->child;
    for (int i = 0; i < size_of_key; i++) {
        if (node->child[key_to_index(key[i])] == NULL) {
            node->child[key_to_index(key[i])] = (node_t *) malloc (sizeof(node_t));
            if (!node->child[key_to_index(key[i])]) {
                return FALSE;
            }
            node = node->child[key_to_index(key[i])];
            memset(node, 0, sizeof(node_t));
            node->key = key[i];
        } else {
            node = node->child[key_to_index(key[i])];
        }
        
    }
    node->value = value;
    node->has_value = TRUE;
    
    return TRUE;
}

/**
 * @brief Lookup the value stored for a particular key in the trie.
 *
 * @details
 * Traverse each level of the trie according to the characters in the 
 * key are return the value stored at the last level.
 *
 * @param[in] trie Pointer to trie.
 * @param[in] key  The key supplied to us.
 * @param[out] value The value stored in the trie for this key.
 *
 * @return Boolean indicating whether the lookup succeded of failed.
 */
boolean lookup_in_trie (trie_t *trie, char *key, int *value)
{
    if (!key_permitted(key)) {
        return FALSE;
    }
    
    node_t *node;
    unsigned int size_of_key;
    
    size_of_key = strlen(key);
    node = trie->child;
    for (int i = 0; i < size_of_key; i++) {
        if (!node->child[key_to_index(key[i])]) {
            return FALSE;
        }
        node = node->child[key_to_index(key[i])];
    }
    if (!node->has_value) {
        return FALSE;
    }
    *value = node->value;
    
    return TRUE;
}

/**
 * @brief Delete the value stored in the trie for a particular key.
 *
 * @details
 * Delete the chain elements for a key if they are only supporting the 
 * value to be deleted. We do so by deleting all the nodes starting at 
 * end all way to a node that either has a value with it or has other 
 * children (supporting other keys).
 *
 * @param[in] trie Poitner to trie.
 * @param[in] key The key supplied to us.
 *
 * @return Boolean indicating if we deleted the key, value pair or not.
 */
boolean delete_from_trie (trie_t *trie, char *key)
{
    node_t *node;
    node_t **nodes_to_be_deleted;
    int index_of_last_node_with_value, starting_index_being_freed;
    boolean index_being_freed_changed;
    
    if (!key_permitted(key)) {
        return FALSE;
    }
    
    int size_of_key, i;
    
    size_of_key = strlen(key);
    node = trie->child;
    nodes_to_be_deleted = (node_t **)malloc(sizeof(node_t) * size_of_key);
    if (nodes_to_be_deleted == NULL) {
        return FALSE;
    }
    
    /*
     * Assignign -1, so that in case where we don't see a node with value in
     * the chain we go on deleting till 0.
     */
    index_of_last_node_with_value = -1;
    index_being_freed_changed = TRUE;
    for (i = 0; i < size_of_key; i++) {
        /*
         * Logic to determine starting_index_being_freed's value -
         * Case 1 - Assume we run into no nodes that are important. In this case, we
         *          want our index to be the key's first character.
         *          index_being_freed_changed will turn TRUE in the last interation
         *          of the for loop but we won't get to the body of for loop. The index
         *          will be the one we encountered during the first iteration of the 
         *          for loop.
         * Case 2 - If we run into a node with a value in the middle,
         *          index_being_freed_changed will help us get the correct value
         *          in the next iteration of the for loop.
         */
        if (index_being_freed_changed) {
            starting_index_being_freed = key_to_index(key[i]);
            index_being_freed_changed = FALSE;
        }
        if (!node->child[key_to_index(key[i])]) {
            goto error_handling;
        }
        node = node->child[key_to_index(key[i])];
        nodes_to_be_deleted[i] = node;
        if ((node->has_value || node_has_multiple_children (node)) && (i != (size_of_key - 1))) {
            index_of_last_node_with_value = i;
            index_being_freed_changed = TRUE;
        }
        
    }
    if (!node->has_value) {
        goto error_handling;
    }
    
    /* 
     * Handle the case where the last element for this key is supporting other elements.
     */
    if (node_has_children(node)) {
        node->has_value = FALSE;
        node->value = 0;
    } else {
        for (i = size_of_key - 1; i > index_of_last_node_with_value; i--) {
            free (nodes_to_be_deleted[i]);
        }
        /*
         * Handle the case where the entire chain is goig away.
         */
        if (index_of_last_node_with_value < 0) {
            trie->child->child[starting_index_being_freed] = NULL;
        } else {
            nodes_to_be_deleted[i]->child[starting_index_being_freed] = NULL;
        }
        free(nodes_to_be_deleted);
    }
    
    return TRUE;
    
error_handling:
    free(nodes_to_be_deleted);
    return FALSE;
    
}

/**
 * @brief Are the characters of of the key permitted?
 * 
 * @param[in] key The key supplied to us.
 * 
 * @return Boolean indicating if the key is ok or not.
 */
static boolean key_permitted (char *key)
{
    unsigned int size_of_key;
    
    size_of_key = strlen(key);
    for (int i = 0; i < size_of_key; i++) {
        if ((key[i] >= 'a') && (key[i] <= 'z')) {
            continue;
        } else {
            return FALSE;
        }
    }
    
    return TRUE;
}

/**
 * @brief Destory the trie, deallocating the assosciate memory.
 * 
 * @note
 * It is expected that all the children of the trie have been deleted.
 * A different implementation may go and delete all the elements via 
 * BFS/DFS too but this doesn't.
 *
 * @param[in, out] trie Pointer to the trie data structure.
 */
void destroy_trie (trie_t *trie)
{
    node_t *node;
    
    node = trie->child;
    
    for (int i = 0; i < NUM_CHILD; i++) {
        assert(node->child[i] == NULL);
    }
    
    free(trie->child);
    free(trie);
}


/**
 * @brief Convert this character to the index of element.
 * @param[in] ch  Character supplied.
 * @return Index to find the corresponding element in the trie.
 */
static unsigned char key_to_index (char ch)
{
    return (ch - 'a');
}


/**
 * @brief Determine if this node has more than one children.
 *
 * @param[in] node Reference to the node.
 * 
 * @return TRUE if this node has more than one children, FALSE otherwise.
 */
static boolean node_has_multiple_children (node_t *node)
{
    unsigned int num_of_children;
    
    if (!node) {
        return FALSE;
    }
    num_of_children = 0;
    for (int i = 0; i < NUM_CHILD; i++) {
        if (node->child[i]) {
            num_of_children++;
            if (num_of_children > 1) {
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

/**
 * @brief Determine if this node has any children.
 *
 * @param[in] node Reference to the node.
 *
 * @return TRUE if this node has any children, FALSE otherwise.
 */
static boolean node_has_children (node_t *node)
{
    if (!node) {
        return FALSE;
    }
    for (int i = 0; i < NUM_CHILD; i++) {
        if (node->child[i]) {
            return TRUE;
        }
    }
    
    return FALSE;
}
