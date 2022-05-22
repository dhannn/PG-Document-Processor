#include "pgdocs.h"
#include "deps/internals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define CONTENT_START_SIGNIFIER "Content"

void _set_infile(Summary *summary, char *filename);
void _set_outfile(Summary *summary, char *filename);
void _close_files(Summary *summary);
void _set_metadata(Summary *summary, int metadataIndex, char *data);
void _set_options(Summary *summary, Config config, int rawInput);
unsigned int _convert_multiselect_options(int rawInput);

typedef enum {
    WORD_COUNT      =       0b00001,
    NGRAM_COUNT     =       0b00010,
    CONCORDANCE     =       0b00100
} ANALYZER_OPTIONS_INDEX;

const AnalyzerOption ANALYZER_OPTIONS[] = {
    {"Word Count", get_word_count},
    {"N-Gram Count", get_ngram_count},
    {"Concordance", get_concordance}
};

int _get_metadata_index(MetadataItem metadataItems[], char *str)
{
    int i;
    int flag = -1;
    
    for(i = 0; i < MAX_METADATA_ITEMS && flag == -1; i++) {
        if(strcmp(metadataItems[i].name, str) == 0)
            flag = i;
    }
    
    return flag;
}

void read_clean_file(Summary *summary, Config config, char *filename)
{
    FILE *infile = summary->inFile;

    int buffSize = BUFSIZ;
    if(config.numChar != 0)
        buffSize = config.numChar;
    
    char buff[buffSize];

    // read metadata
    int flag = -1;
    
    // sets the position to metadata information
    // TODO: seek metadata
    while(flag == -1) {
        fscanf(infile, "%s", buff);

        buff[strlen(buff) - 1] = '\0';

        flag = _get_metadata_index(summary->metadata.metadataItems, buff);
    }

    fseek(infile, -(strlen(buff)) - 1, SEEK_CUR);

    // TODO: extract read metadata method
    do {
        flag = fscanf(infile," %[^:]: s", buff);

        int index = _get_metadata_index(summary->metadata.metadataItems, buff);
        
        // TODO: extract constant
        if(index > -1) {
            flag = fscanf(infile,"%[^\n]s", buff);
            _set_metadata(summary, index, buff);
        }
    } while(strcmp(buff, CONTENT_START_SIGNIFIER) && flag != EOF);
    
    
    // TODO: extract read content method
    // Read content
    int runningTotal = 0;
    char *strTemp = calloc(1, sizeof(char));
    while(flag != EOF) {
        flag = fscanf(infile, "%s", buff);

        runningTotal += strlen(buff) + 1;
        strTemp = realloc(strTemp, runningTotal + 1);

        strcat(buff, " ");
        strcat(strTemp, buff);
    }
    
    summary->inData = strTemp;
    summary->tokenList = tokenize_string(summary->inData, false);
}

void analyze_data(Summary *summary, Config config)
{
    unsigned int options = summary->options;

    for(int i = 0; i < MAX_ANALYZER_OPTIONS; i++) {
        int current = pow(2, i);

        if((current & options) != 0) {
            ANALYZER_OPTIONS[i].do_analysis(summary, config);
            // ANALYZER_OPTIONS[i].report_analysis(summary, config);
        }
    }
}

TokenList *remove_duplicate_tokens(TokenList *tl)
{
    HashTable *ht = create_hash_table();
    
    TokenList *cleanedTl = initialize_tokenlist();
    TokenNode *currentNode = tl->head;

    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(*currentTokenStr == ' ') {
            currentNode = currentNode->next;
            continue;
        }

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
            add_token(cleanedTl, currentTokenStr);
        }

        currentNode = currentNode->next;
    }

    destroy_hash_table(ht);

    return cleanedTl;
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

    print_tokens(summary->tokenList);

    // TODO: abstract away the deallocation of shit
    delete_tokens(tokensWithDuplicates);
    free(summary->inData);
    for(int i = 0; i < MAX_METADATA_ITEMS; i++) {
        free(summary->metadata.metadataItems[i].data);
    }
    destroy_hash_table(ht);
    destroy_tokenList(tokensWithDuplicates);
    destroy_tokenList(summary->tokenList);
    fclose(summary->inFile);
}

void get_ngram_count(Summary *summary, Config config)
{
    HashTable *ht = create_hash_table();

    TokenList *rawTokens = convert_to_ngrams(summary->tokenList, 2);
    TokenList *ngrams = remove_duplicate_tokens(rawTokens);

    // print_tokens(summary->tokenList);

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
    destroy_tokenList(rawTokens);
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

// TODO: deallocate metadata
void _set_metadata(Summary *summary, int metadataIndex, char *data)
{
    char *temp = malloc(strlen(data) + 1);
    strcpy(temp, data);
    summary->metadata.metadataItems[metadataIndex].data = temp;
}

void _set_options(Summary *summary, Config config, int rawInput)
{
    if(config.multiSelect == false) {
        summary->options = (int)pow(2, rawInput - 1);
        return;
    }

    summary->options = _convert_multiselect_options(rawInput);
}

unsigned int _convert_multiselect_options(int rawInput)
{
    // a lookup table to prevent duplicates
    bool isEntered[MAX_ANALYZER_OPTIONS];

    for(int i = 0; i < MAX_ANALYZER_OPTIONS; i++) 
        isEntered[i] = false;

    
    unsigned int options = 0;
    while(rawInput > 0) {
        // gets rightmost digit
        int currDigit = rawInput % 10;

        // only adds the option to bitfield only when not added
        if(isEntered[currDigit] == false) {
            options += (int)pow(2, currDigit - 1);
            isEntered[currDigit] = true;
        }

        // removes rightmost digit
        rawInput /= 10;
    }

    return options;
}
