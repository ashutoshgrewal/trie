//
//  main.c
//  trie
//
//  Created by Ashutosh Grewal on 12/4/16.
//  Copyright © 2016 Ashutosh Grewal. All rights reserved.
//
/*
 * Copyright © 2016 Ashutosh Grewal. All rights reserved.
 *
 * @file main.c
 * @author Ashutosh Grewal on 12/10/16.
 *
 * @brief This file just contains some code that uses the APIs of
 * trie data structure
 *
 * @declaration
 * The forward declaration of trie only allows to create pointers
 * to this struct and not the struct itself while the definition of
 * boolean - as expected - doesn't put any restrictions.
 */

#include <stdio.h>
#include "trie.h"

int main(int argc, const char * argv[]) {

    trie_t *trie;
    int value;
    boolean result;
    char *string;
    
    trie = create_trie();
    
    add_to_trie("aa", 1, trie);
    add_to_trie("ab", 2, trie);
    add_to_trie("ac", 3, trie);
    
    string = "aa";
    result = lookup_in_trie(trie, string, &value);
    printf("Looking up for %s...\n", string);
    if (result) {
        printf("\tfound with value - %d\n", value);
    } else {
        printf("\tdid not find");
    }
    string = "ab";
    result = lookup_in_trie(trie, string, &value);
    printf("Looking up for %s...\n", string);
    if (result) {
        printf("\tfound with value - %d\n", value);
    } else {
        printf("\tdid not find");
    }
    string = "ac";
    result = lookup_in_trie(trie, string, &value);
    printf("Looking up for %s...\n", string);
    if (result) {
        printf("\tfound with value - %d\n", value);
    } else {
        printf("\tdid not find");
    }
    string = "ad";
    result = lookup_in_trie(trie, string, &value);
    printf("Looking up for %s...\n", string);
    if (result) {
        printf("\tfound with value - %d\n", value);
    } else {
        printf("\tdid not find.");
    }
    
    string = "ab";
    result = delete_from_trie(trie, string);
    add_to_trie("aaak", 10, trie);
    string = "aaak";
    result = lookup_in_trie(trie, string, &value);
    printf("Looking up for %s...\n", string);
    if (result) {
        printf("\tfound with value - %d\n", value);
    } else {
        printf("\tdid not find.");
    }
    string = "aa";
    result = delete_from_trie(trie, string);
    string = "ac";
    result = delete_from_trie(trie, string);
    string = "ab";
    result = delete_from_trie(trie, string);
    string = "aaak";
    result = delete_from_trie(trie, string);
    destroy_trie(trie);
    return 0;
}


