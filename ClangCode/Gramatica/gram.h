#include <stdio.h>
#include <stdlib.h>

typedef struct gramatica{
    char *E;
    char *N;
    char *P;
    char *S;
} gramatica;

typedef struct Node {
    struct Node *esq;
    struct Node *dir;
    char *token;
} Node;


int testvar(gramatica gram, char c, size_t opt)// 1 for N rules, 2 for alphabet, 3 for both
{
    if(c == ':')
        return 0;
    int j, i, d;
    if(opt == 1 || opt == 3)
    {
        if (gram.S[0] == c) 
        {
            return 0; // Match found
        }
        for(j = 0; gram.N[j] != '\0'; j++)
        {
            if(gram.N[j] == c)
            {
                return 0; // Match found
            }
        }
    }
    
    if(opt == 2 || opt == 3)
    {
        for(i = 0; gram.E[i] != '\0'; i++)
        {
            if(gram.E[i] == c)
            {
                return 0; // Match found
            }
        } 
    }
    for(d = 0; d <= i && d <= j; d++)
        {
            printf("%c nao existe em E = %c ou N = %c\n", c, gram.E[d], gram.N[d]);
        } 
    
    return 1; // No match found
}

bool test_rule_product(char *rule, char *word, gramatica gram) 
{
    char buff[100];
    int i = 0;
    char *temp = (char *)malloc(100 * sizeof(char)); // Allocate memory for temp

    // Find the beginning of the rule
    while (*rule != ':')
        rule++;
    
    // Loop through the rule to copy products into buff
    for (i = 0; *rule != ';' && *rule != '\0'; rule++) 
    {
        if (!testvar(gram, *rule, 2))
            continue; // Skip recording characters that fail the testvar check
                      
        buff[i++] = *rule;
    }

    buff[i] = '\0'; // Null-terminate the buff string

    // Copy buff into temp
    strncpy(temp, buff, sizeof(buff) - 1);
    temp[sizeof(buff) - 1] = '\0'; // Ensure temp is null-terminated

    // Compare temp with word
    if (strcmp(temp, word) != 0) 
    {
        free(temp); // Free dynamically allocated memory
        return false;
    }

    free(temp); // Free dynamically allocated memory
    return true;
}

void print_tree(Node *root)
{
    if (root == NULL) {
        printf("Root node is NULL\n");
        return;
    }

    if (root->token == NULL) {
        printf("Token: (null)\n");
    } else {
        printf("Token: %s\n", root->token); // Print the current node's token
    }

    if(root->esq != NULL)
    {    
        printf("Printing left subtree:\n");
        print_tree(root->esq);
    }
    if(root->dir != NULL)
    {
        printf("Printing right subtree:\n");
        print_tree(root->dir);
    }
    return;
}

void free_tree(Node *root) {
    int n = 0;

    if (root == NULL) {
        return;
    }

    // Free memory for the left subtree
    free_tree(root->esq);

    // Free memory for the right subtree
    free_tree(root->dir);

    // Free memory for the token
    if (root->token != NULL) {
        free(root->token);
    }

    // Free memory for the node itself
    free(root);
}
