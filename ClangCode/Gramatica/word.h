#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif


char ** find_rule(char *, char , const int );
void process_word(Node *, gramatica , char *, size_t );

// To be used in this function
//    p = find_rule(gram.P, c);
//    while(*(*p) != '\0')
//    {
//        printf("\n MATCH %c\n", *(*p));
//        *p++;
//    }


void process_word(Node *root, gramatica gram, char *word, size_t w)
{ // for reference process_word(palavra, gram, word, strlen(word))
    //rework whole function. 
    int i = 0, j = 0;
    char c[w];
    char *temp = (char *)malloc(sizeof(char *));
    char **matches = (char **)malloc(100 * sizeof(char **));
    
    if(root == NULL)
        root = (Node *)malloc(sizeof(Node *));

    if(root == NULL)
        return;

    if(root->token != NULL)
        goto end;

    
    matches = find_rule(gram.P, *word, 0);
    
    while(matches[i] != NULL)
    {
        if(!test_rule_product(matches[i], word, gram))
        {
            matches[i] = NULL;
            continue;
        }
        while(*matches[i] != ':')
            matches[i]++;
        while(*matches[i] != ';')
        {
            root->token = (char *)malloc(sizeof(char *));
            *root->token = *matches[i];
            root->token++;
            matches[i]++;
        }
    }
    
end:
    return process_word(root->esq, gram, word, strlen(word));
    return process_word(root->dir, gram, word, strlen(word));
}

char ** find_rule(char *rules, char var, const int n)
{// 1 to find rule and 0 to find product
    bool s = false;
    char **p = NULL;
    int match_count = 0;
   
    printf("\nEsta em find_rule\n");
    p = (char **)malloc(strlen(rules) * sizeof(char *)); // Allocate memory for array of pointers

    do 
    {
        p[match_count] = (char *)malloc(2 * sizeof(char)); // Allocate memory for a character and null terminator

        switch (*rules) 
        {
            case ':':
                s = true;
                break;
            case ';':
                s = false;
                break;
        }

        if (*rules == var && !s && n == 1) {
            char *temp = rules;

            while (*temp != ';') 
                temp--;
            
            temp++; 
            p[match_count] = temp;
            printf("Value is in char %c and in memory %p\n", *temp, (void *)temp);
            match_count++;
        } 
        if (*rules == var && s && n == 0) 
        {
            char *temp = rules;
            
            while (*temp != ';') 
                temp--;
            
            temp++;
            p[match_count] = temp;
            printf("Value is in char %c and in memory %p\n", *temp, (void *)temp);
            match_count++;
        } 
        rules++;
    } while (*rules != '\0'); // Make sure not to exceed array bounds

    // Set the last pointer in the array to NULL to signify the end
    p[match_count] = NULL;

    // Free any unused memory
    p = realloc(p, (match_count + 1) * sizeof(char *));

    printf("\nSuccess for alloc\n");
    return p;
}


