/*******************************************************************************
 * 
 * FILE             analyzer.c
 * LAST MODIFIED    06-17-2022
 * 
 * DESCRIPTION
 *      This file contains the function implementations that process
 *      set tokenlists to extract features like word count, tf-idf 
 *      and n-gram count.
 * 
 ******************************************************************************/

#include "pgdocs.h"
#include "deps/internals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/**
 * convert_to_ngrams()
 * breaks down the tokenlist given n tokens
 * 
 * @param       TokenList*      the list of raw tokens to be processed
 * @param       int             the number of tokens in a sequence
 * @return      a tokenlist forming the ngrams
 */
TokenList *__convert_to_ngrams(TokenList *tl, int n);
/**
 * get_idf()
 * calculates the inverse document frequency of a given term
 * 
 * @param       TokenList**     a list of tokens of the whole corpus
 * @param       char*           keyword to be searched
 */
float __get_idf(TokenList **corpusTokens, char *tokenString);

/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
/* ---------------------------- GETTING FUNCTIONS --------------------------- */
/*                         gets the analyzing features                        */
void get_word_count(Summary *summary)
{
    HashTable *ht = create_hash_table();
    
    TokenList *tokensWithDuplicates = summary->tokenList;
    TokenList *tokensWithoutDuplicates = remove_duplicate_tokens(tokensWithDuplicates);
    TokenNode *currentNode = tokensWithDuplicates->head;
    int numTokens = 0;

    summary->maxTokenChar = strlen(currentNode->tokenString);

    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
        } else {
            increment_token_frequency(tokensWithoutDuplicates, currentTokenStr);
        }

        if(strlen(currentTokenStr) > summary->maxTokenChar)
            summary->maxTokenChar = strlen(currentTokenStr);

        numTokens++;
        update_processing(numTokens, summary->tokenList->size);

        currentNode = currentNode->next;
    }

    summary->tokenList = tokensWithoutDuplicates;

    sort_tokens_by_freq(summary->tokenList);

    destroy_hash_table(ht);
    delete_token_strings(tokensWithDuplicates);
    destroy_tokenList(tokensWithDuplicates);
}

void get_ngram_count(Summary *summary)
{
    HashTable *ht = create_hash_table();
    
    int n = summary->addOpts.i[0];
    TokenList *rawTokens = summary->tokenList;
    TokenList *rawNgrams = __convert_to_ngrams(summary->tokenList, n);
    TokenList *ngrams = remove_duplicate_tokens(rawNgrams);
    int numTokens = 0;

    summary->maxTokenChar = strlen(ngrams->head->tokenString);    

    TokenNode *currentNode = rawNgrams->head;
    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
        } else {
            increment_token_frequency(ngrams, currentTokenStr);
        }

        if(strlen(currentTokenStr) > summary->maxTokenChar)
            summary->maxTokenChar = strlen(currentTokenStr);

        currentNode = currentNode->next;

        numTokens++;
        update_processing(numTokens, summary->tokenList->size);
    }

    summary->tokenList = ngrams;
    sort_tokens_by_freq(summary->tokenList);

    destroy_hash_table(ht);

    delete_token_strings(rawTokens);
    destroy_tokenList(rawTokens);

    delete_token_strings(rawNgrams);
    destroy_tokenList(rawNgrams);
}

void get_concordance(Summary *summary)
{
    AdditionalOptions addOpts = summary->addOpts;
    char *keyword = addOpts.s[0];
    int n = addOpts.i[0];

    TokenList *oldTokenlist = summary->tokenList;
    TokenList *newTokenlist = initialize_tokenlist();

    TokenNode *curr = oldTokenlist->head;
    TokenNode *startOfWindow = curr;

    for(int i = 0; i < n && curr != NULL; i++) {
        curr = curr->next;
    }

    while(curr != NULL) {
        if(strcmp(curr->tokenString, keyword) == 0) {
            char buff[MAX_CHAR] = "";
            int num = 0;
            TokenNode *tempNode = startOfWindow;
            
            // concatenate the words before the keyword
            for(int i = 0; i < n; i++) {
                strcat(buff, tempNode->tokenString);
                strcat(buff, " ");

                tempNode = tempNode->next;
                num++;
            }

            // concatenate the keyword and the words after it
            for(int i = 0; i <= n && (tempNode + i) != NULL; i++) {
                strcat(buff, tempNode->tokenString);

                if(i != n)
                    strcat(buff, " ");
                
                tempNode = tempNode->next;
                num++;
            }

            // only add the concatenated buffer if complete 
            // (i.e. n words before and n words after)
            if(num == 2 * n + 1) {
                int length = strlen(buff);
                char *temp = calloc(length + 1, 1);
                
                strcpy(temp, buff);
                add_token(newTokenlist, temp);
            }

        }

        startOfWindow = startOfWindow->next;
        curr = curr->next;
    }

    summary->tokenList = newTokenlist;

    delete_token_strings(oldTokenlist);
    destroy_tokenList(oldTokenlist);
}

void get_tfidf(Summary *summary)
{
    get_word_count(summary);

    TokenList *tokenlist = summary->tokenList;
    TokenNode *currentNode = tokenlist->head;
    
    float maxFreq = (float)currentNode->frequency;
    while(currentNode != NULL) {
        float tf = 0.5 + 0.5 * ((float)currentNode->frequency / maxFreq);
        float idf = __get_idf(summary->corpusTokens, currentNode->tokenString);

        currentNode->tfidf = tf * idf;
        currentNode = currentNode->next;
    }

    sort_tokens_by_tfidf(summary->tokenList);
}

/* --------------------------- REPORTING FUNCTIONS -------------------------- */
/*                       writes the output to the string                      */
void report_token_frequency(Summary *summary)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int runningTotal = 0;
    int currentSize = MAX_CHAR;
    char *temp = calloc(currentSize, 1);
    char buff[MAX_CHAR] = "";

    int numChar = 0;

    while(tokenNode != NULL) {
        sprintf(buff, "%-*s\t% 5d\n", summary->maxTokenChar * 3 / 4, tokenNode->tokenString, tokenNode->frequency);
        buff[strlen(buff)] = '\0';

        runningTotal += strlen(buff);

        if(runningTotal >= currentSize) {
            currentSize *= 5;
            temp = realloc(temp, currentSize);
        }

        strcat(temp, buff);                
        temp[strlen(temp)] = '\0';
        
		numChar++;
		update_reporting(numChar, summary->tokenList->size);

        tokenNode = tokenNode->next;
    }

    summary->outData = temp;
}

void report_ngram_count(Summary *summary)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int n = 0;
    while(tokenNode != NULL) {
        if(n < 10)
            fprintf(stdout, "%s:\t\t% 5d\n", tokenNode->tokenString, tokenNode->frequency);

        fprintf(summary->outfile, "%s: % 5d\n", tokenNode->tokenString, tokenNode->frequency);
        
        tokenNode = next_token(list);
        n++;
    }
}

void report_concordance(Summary *summary)
{
    TokenNode *curr = summary->tokenList->head;

    int size = strlen(curr->tokenString);
    char *temp = calloc(size, 1);
    char buff[MAX_CHAR] = "";
    int runningTotal = 0;

    while(curr != NULL) {
        int length = strlen(curr->tokenString);
        runningTotal += length + 1;

        if(runningTotal > size) {
            size *= 2;
            temp = realloc(temp, size);
        }

        sprintf(buff, "%s\n", curr->tokenString);
        strcat(temp, buff);
        curr = curr->next;
    }

    summary->outData = temp;
}

void report_tfidf(Summary *summary)
{
    TokenList *list = summary->tokenList;
    TokenNode *tokenNode = next_token(list);

    int runningTotal = 0;
    int currentSize = MAX_CHAR;
    char *temp = calloc(currentSize, 1);
    char buff[MAX_CHAR] = "";

    int numChar = 0;

    while(tokenNode != NULL) {
        sprintf(buff, "%20s:\t% 5.2f\n", tokenNode->tokenString, tokenNode->tfidf);
        buff[strlen(buff)] = '\0';

        runningTotal += strlen(buff);

        if(runningTotal + 1 > currentSize) {
            currentSize *= 5;
            temp = realloc(temp, currentSize);
        }

        strcat(temp, buff);
        temp[strlen(temp)] = '\0';
        
		numChar++;
		update_reporting(numChar, summary->tokenList->size);

        tokenNode = tokenNode->next;
    }

    summary->outData = temp;
}

/* -------------------------------------------------------------------------- */
/*                              PRIVATE FUNCTIONS                             */
/* -------------------------------------------------------------------------- */

TokenList *__convert_to_ngrams(TokenList *tl, int n)
{
    TokenList *ngrams = initialize_tokenlist();
    TokenNode *curr = tl->head;
    char buff[MAX_CHAR];

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

float __get_idf(TokenList **corpusTokens, char *tokenString)
{
    float idf = 0;
    int numDocsWithKeyword = 0;

    int i = 0;
    for(i = 0; corpusTokens[i] != NULL; i++) {
        if(is_token_found(corpusTokens[i], tokenString))
            numDocsWithKeyword++;
    }

    if(numDocsWithKeyword != 0)
        idf = log10f((float)i / numDocsWithKeyword);

    return idf;
}
