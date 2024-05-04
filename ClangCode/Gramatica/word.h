#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

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
    do {
        printf("\nIn loop\n");
        p = (char **)malloc(sizeof(char *)); // Allocate memory for array of pointers
        switch (*rules) {
            case ':':
                s = true;
                break;
            case ';':
                s = false;
                break;
        }

        if (*rules == var && s == false) {
            p[match_count] = (char *)malloc(sizeof(char)); // Allocate memory for the character
            *(p[match_count]) = var;
            printf("Value is in char %c and in int %i\n", var, var);
            match_count++;
        }else{
            printf("\nRegra %c e diferente de var %c\n", *rules, var);
        }
        rules++;
    } while (*rules != '\n'); // Make sure not to exceed array bounds

    return p;
}
