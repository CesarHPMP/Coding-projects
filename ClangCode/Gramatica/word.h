#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

// To be used in this function
//    p = find_rule(gram.P, c);
//    while(*(*p) != '\0')
//    {
//        printf("\n MATCH %c\n", *(*p));
//        *p++;
//    }


void process_word(Node *root, gramatica gram, char *word, size_t w) { // for reference process_word(palavra, &arvore, gram, word)
    //rework whole function. 
    int i = 0, j = 0;
    char c[w];
    root = (Node *)malloc(sizeof(Node *));

    for(i = 0; *word != '\0'; i++)
    {
        c[i] = *word;
        word++;
    }    
}

char ** find_rule(char *rules, char var) {
    bool s = false;
    char **p = NULL;
    int match_count = 0;
   
    printf("\nEsta em find_rule\n");
    p = (char **)malloc(strlen(rules) * sizeof(char *)); // Allocate memory for array of pointers

    do {
        printf("\nIn loop\n");
        p[match_count] = (char *)malloc(2 * sizeof(char)); // Allocate memory for a character and null terminator
        switch (*rules) {
            case ':':
                s = true;
                break;
            case ';':
                s = false;
                break;
        }

        if (*rules == var && s == false) {
            *(p[match_count]) = var;
            *(p[match_count] + 1) = '\0'; // Null-terminate the string
            printf("Value is in char %c and in int %i\n", var, var);
            match_count++;
        } else if (s == false) {
            printf("\nRegra %c e diferente de var %c\n", *rules, var);
        } else {
            printf("NAO E REGRA DE PRODUCAO MAS SIM PRODUTO");
        }
        rules++;
    } while (*rules != '\0'); // Make sure not to exceed array bounds

    // Null-terminate the array
    *p[match_count] = '\0';

    // Free any unused memory
    p = realloc(p, (match_count + 1) * sizeof(char *));

    printf("\nSuccess for alloc\n");
    return p;
}

