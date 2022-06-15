#include "pgdocs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

#define NOT_METADATA -1

#define CLEAN_CONTENT_START_SIGNIFIER "*** START"
#define ANALYZE_CONTENT_START_SIGNIFIER "Content"
#define CONTENT_END_SIGNIFIER "*** END"

char *__get_starting_token(ModeIndex mode);
bool __check_if_content_start(char buff[], ModeIndex mode);
bool __check_if_content_end(char buff[], char prev[]);
bool __check_if_max_char_reached(int numCharRead, int numCharMax);

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

    int i;
    int aBeautifulVariableName = 1;

    char buff[MAX_CHAR] = "";
    
    for(i = 0; corpus[i] != NULL; i++) {
        fscanf(corpus[i] , "%s", buff);

        while(!__check_if_content_start(strtok(buff, ":"), ANALYZE_MULTI)){
            fscanf(corpus[i] , "%s", buff);
        }
        
        if(i >= aBeautifulVariableName) {
            aBeautifulVariableName *= 5;
            temp = realloc(temp, sizeof(char*) * aBeautifulVariableName);
        }

        read_content(corpus[i], &temp[i], maxChar);
    }

    temp[i] = NULL;

    summary->corpusData = temp;
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
    char *temp = calloc(1, sizeof(char));   // allocation for the content string

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

char *__get_starting_token(ModeIndex mode)
{
    if(mode == CLEAN)
        return CLEAN_CONTENT_START_SIGNIFIER;
        
    return ANALYZE_CONTENT_START_SIGNIFIER;
}

bool __check_if_content_start(char buff[], ModeIndex mode)
{
    char *startingToken = __get_starting_token(mode);
    int len = strlen(startingToken);
    buff[len] = '\0';

    return strcmp(startingToken, buff) == 0;
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

    char *startingToken = CONTENT_END_SIGNIFIER;
    int len = strlen(startingToken);
    temp[len] = '\0';

    return strcmp(startingToken, temp) == 0;
}

