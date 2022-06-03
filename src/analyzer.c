#include "pgdocs.h"
#include "deps/internals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_ANALYZER_OPTIONS 3

unsigned int _convert_multiselect_options(int rawInput);

typedef enum {
    WORD_COUNT      =       0b00001,
    NGRAM_COUNT     =       0b00010,
    CONCORDANCE     =       0b00100
} ANALYZER_OPTIONS_INDEX;

void analyze_data__single(Summary *summary, Config config)
{
    unsigned int options = summary->options;

    for(int i = 0; i < MAX_ANALYZER_OPTIONS; i++) {
        int current = pow(2, i);

        if((current & options) != 0) {
            // ANALYZER_OPTIONS[i].do_analysis(summary, config);
            // ANALYZER_OPTIONS[i].report_analysis(summary, config);
        }
    }
}

TokenList *convert_to_ngrams(TokenList *tl, int n)
{
    // TODO extract constant
    if(n < 2 || n > 4) {
        return NULL;
    }

    TokenList *ngrams = initialize_tokenlist();
    TokenNode *curr = tl->head;
    char buff[BUFSIZ];

    while(curr != NULL) {
        strcpy(buff, curr->tokenString);
        
        int i;
        TokenNode *window = curr->next;
        for(i = 1; i < n && window != NULL; i++) {
            strcat(buff, " ");
            strcat(buff, window->tokenString);
            window = window->next;
        }

        if(i == n) {
            strcat(buff, "\0");

            char *temp = calloc(strlen(buff) + 1, sizeof(char));
            strcpy(temp, buff);

            add_token(ngrams, temp);
        }

        curr = curr->next;
    }
    
    return ngrams;
}

void get_word_count(Summary *summary, Config config)
{
    HashTable *ht = create_hash_table();
    
    // uncleaned list = with duplicates
    TokenList *tokensWithDuplicates = summary->tokenList;

    // cleaned list = without duplicates
    TokenList *tokensWithoutDuplicates = remove_duplicate_tokens(tokensWithDuplicates);
    TokenNode *currentNode = tokensWithDuplicates->head;

    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
        } else {
            increment_token_frequency(tokensWithoutDuplicates, currentTokenStr);
        }

        currentNode = currentNode->next;
    }

    summary->tokenList = tokensWithoutDuplicates;
    
    sort_tokens(summary->tokenList);

    destroy_hash_table(ht);
    // delete_token_strings(tokensWithDuplicates); // keep

    // TODO: abstract away the deallocation of shit
    // free(summary->inData); // [/]
    // for(int i = 0; i < MAX_METADATA_ITEMS; i++) {
    //     free(summary->metadata.metadataItems[i].data);
    // }
    
    destroy_tokenList(tokensWithDuplicates);
    // destroy_tokenList(summary->tokenList);
    fclose(summary->inFile);
}

void get_ngram_count(Summary *summary, Config config)
{
    HashTable *ht = create_hash_table();

    TokenList *rawTokens = convert_to_ngrams(summary->tokenList, 2);
    TokenList *ngrams = remove_duplicate_tokens(rawTokens);

    TokenNode *currentNode = rawTokens->head;
    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
        } else {
            increment_token_frequency(ngrams, currentTokenStr);
        }

        currentNode = currentNode->next;
    }

    destroy_tokenList(summary->tokenList);
    summary->tokenList = ngrams;

    sort_tokens(summary->tokenList);
    destroy_tokenList(rawTokens);
}

void report_token_frequency(Summary *summary, Config config)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int n = 0;
    while(tokenNode != NULL) {
        if(n < 10)
            fprintf(stdout, "%s: %d\n", tokenNode->tokenString, tokenNode->frequency);

        fprintf(summary->outFile, "%s: %d\n", tokenNode->tokenString, tokenNode->frequency);
        
        tokenNode = next_token(list);
        n++;
    }
}

void report_ngram_count(Summary *summary, Config config)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int n = 0;
    while(tokenNode != NULL) {
        if(n < 10)
            fprintf(stdout, "%s: %d\n", tokenNode->tokenString, tokenNode->frequency);

        fprintf(summary->outFile, "%s: %d\n", tokenNode->tokenString, tokenNode->frequency);
        
        tokenNode = next_token(list);
        n++;
    }
}

void report_concordance(Summary *summary, Config config)
{

}

void get_concordance(Summary *summary, Config config)
{
    
}

void _set_infile(Summary *summary, char *filename)
{
    summary->inFile = fopen(filename, "r");
}

void _set_outfile(Summary *summary, char *filename)
{
    summary->outFile = fopen(filename, "r");
}

void _close_files(Summary *summary)
{
    fclose(summary->inFile);
    fclose(summary->outFile);
}
