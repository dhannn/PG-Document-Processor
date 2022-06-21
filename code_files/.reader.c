/*******************************************************************************
 *                                                                             
 * FILE             .reader.c
 * LAST MODIFIED    06-16-2020
 * 
 * DESCRIPTION
 *      This file contains the function implementations involving reading
 *      metadata and content from the specified text files.
 * 
 ******************************************************************************/

#include "pgdocs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

#define NOT_METADATA -1

#define CLEAN_CONTENT_START_SIGNIFIER "*** START"
#define CLEAN_CONTENT_ALT_START_SIGNIFIER "***START"
#define ANALYZE_CONTENT_START_SIGNIFIER "Content"
#define CONTENT_END_SIGNIFIER "*** END"
#define CONTENT_END_ALT_SIGNIFIER "***END"

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/**
 * get_starting_token()
 * gets the token signifying the start of content given the mode
 * 
 * @param           ModeIndex       the mode of the reading
 * @return          string of the starting token
 */
char *__get_starting_token(ModeIndex mode);
/**
 * check_if_content_start()
 * checks if the current pointer to file is the start of the content
 * 
 * @param           char[]          current string token
 * @param           ModeIndex       the mode of the reading
 * @return          true if pointer is content start; false if otherwise
 */
bool __check_if_content_start(char buff[], ModeIndex mode);
/**
 * check_if_content_end()
 * checks if the current pointer to file is the end of the content
 * 
 * @param           char[]          current string token
 * @param           char[]          previous string token
 * @return          true if pointer is content end; false if otherwise
 */
bool __check_if_content_end(char buff[], char prev[]);
/**
 * check_if_max_char_reached()
 * checks if the reader reached the maximum number of characters
 * 
 * @param           int             number of characters read
 * @param           int             maximum number of characters
 * @return          true if maximum character is reached; false if otherwise
 */
bool __check_if_max_char_reached(int numCharRead, int numCharMax);

/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

void read_file(Summary *summary, Config config)
{
    FILE *infile = summary->infile;

    seek_metadata(infile, summary->metadata);
    read_metadata(infile, summary->metadata, summary->mode.index);
    read_content(infile, &summary->inData, config.numChar);
}

void read_corpus(Summary *summary, Config config) 
{
    int maxChar = config.numChar;

    FILE **corpus = summary->corpus;
    char **temp = malloc(sizeof(char*));

    int i = 0;
    int aBeautifulVariableName = 1;

    char buff[MAX_CHAR] = "";

    while(corpus[i] != NULL) {
        fscanf(corpus[i] , "%s", buff);

        while(!__check_if_content_start(strtok(buff, ":"), ANALYZE_MULTI)){
            fscanf(corpus[i] , "%s", buff);
        }
        
        if(i >= aBeautifulVariableName) {
            aBeautifulVariableName *= 5;
            temp = realloc(temp, sizeof(char*) * aBeautifulVariableName);
        }

        read_content(corpus[i], &temp[i], maxChar);

        i++;
    }
        
    if(i >= aBeautifulVariableName) {
        aBeautifulVariableName *= 5;
        temp = realloc(temp, sizeof(char*) * aBeautifulVariableName);
    }

    temp[i] = NULL;

    summary->corpusData = temp;
}

void read_compfile(Summary *summary, Config config)
{
    int maxChar = config.numChar;
    FILE *compfile = summary->compfile;
    char buff[MAX_CHAR];
    char *temp;

    fscanf(compfile , "%s", buff);

    while(!__check_if_content_start(strtok(buff, ":"), ANALYZE_MULTI)){
        fscanf(compfile, "%s", buff);
    }

    read_content(compfile, &temp, maxChar);
    summary->compData = temp;
}

void seek_metadata(FILE *file, MetadataItem items[])
{
    int flag = NOT_METADATA;
    char buff[MAX_CHAR];

    // increments the pointer until a metadata item is found
    while(flag == NOT_METADATA) {
        fscanf(file, "%s", buff);

        buff[strlen(buff) - 1] = '\0';

        flag = get_metadata_index(items, buff);
    }

    // redirects current pointer right before the first metadata item
    fseek(file, -(strlen(buff)) - 1, SEEK_CUR);
}

void read_metadata(FILE *file, MetadataItem items[], ModeIndex mode)
{
    char buff[MAX_CHAR];
    int flag;

    do {
        flag = fscanf(file, " %[^:\n]: s", buff);
        int index = get_metadata_index(items, buff);
        
        if(index != NOT_METADATA) {
            flag = fscanf(file, "%[^\n]s", buff);
            set_metadata(items, index, buff);
        }

    } while(!__check_if_content_start(buff, mode) && flag != EOF);

}

void read_content(FILE *file, char **inputData, int maxChar)
{
    int runningTotal = 0;                   // total number of characters
    int currentSize = MAX_CHAR;
    
    // allocation for the content string
    char *temp = calloc(currentSize, sizeof(char));

    char buff[MAX_CHAR];
    char prev[MAX_CHAR] = "";               // to test for "*** END" signifier
    int numWords = 0;                       // number of words read for update()

    int flag = fscanf(file, "%s", buff);    // flag to determine if EOF

    while(flag != EOF) {
        bool isContentEnd = __check_if_content_end(buff, prev);
        bool isMaxReached = __check_if_max_char_reached(runningTotal, maxChar);

        if(isContentEnd || isMaxReached)
            break;

        strcat(buff, " ");
        runningTotal += strlen(buff);

        if(runningTotal >= currentSize) {
            currentSize *= 5;
            temp = realloc(temp, currentSize); 
        }

        strcat(temp, buff);

        update_reading(runningTotal, numWords);
        strcpy(prev, buff);
        numWords++;

        flag = fscanf(file, "%s", buff);
    }

    *inputData = temp;
}

void clean_up_reader(Summary *summary)
{
    delete_metadata(summary->metadata);
    free(summary->inData);
}

/* -------------------------------------------------------------------------- */
/*                              PRIVATE FUNCTIONS                             */
/* -------------------------------------------------------------------------- */

char *__get_starting_token(ModeIndex mode)
{
    if(mode == CLEAN)
        return CLEAN_CONTENT_START_SIGNIFIER;
        
    return ANALYZE_CONTENT_START_SIGNIFIER;
}

char *__get_alt_starting_token(ModeIndex mode)
{
    return CLEAN_CONTENT_ALT_START_SIGNIFIER;
}

bool __check_if_content_start(char buff[], ModeIndex mode)
{
    char *startingToken = __get_starting_token(mode);
    int len = strlen(startingToken);
    buff[len] = '\0';

    bool flag = strcmp(startingToken, buff) == 0;

    if(!flag && mode == CLEAN) {
        flag = strcmp(CLEAN_CONTENT_ALT_START_SIGNIFIER, buff) == 0;
    }

    return flag;
}

bool __check_if_max_char_reached(int numCharRead, int numCharMax)
{
    return numCharMax != 0 && numCharRead > numCharMax;
}

bool __check_if_content_end(char buff[], char prev[])
{
    char temp[MAX_CHAR] = "";
    strcpy(temp, prev);
    strcat(temp, buff);

    char *endingToken = CONTENT_END_SIGNIFIER;
    char *altEndingToken = CONTENT_END_SIGNIFIER;
    int len = strlen(endingToken);
    temp[len] = '\0';

    return strcmp(endingToken, temp) == 0;
}

