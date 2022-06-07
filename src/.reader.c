#include "pgdocs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NOT_METADATA -1

#define CLEAN_CONTENT_START_SIGNIFIER "*** START"
#define ANALYZE_CONTENT_START_SIGNIFIER "Content"
#define CONTENT_END_SIGNIFIER "*** END"

char *__get_starting_token(ModeIndex mode);
bool __check_if_content_start(char buff[], ModeIndex mode);
bool __check_if_content_end(char buff[], char prev[]);

void read_file(Summary *summary, Config config)
{
    FILE *infile = summary->inFile;
    int buffSize = MAX_CHAR;

    if(config.numChar != 0)
        buffSize = config.numChar;

    seek_metadata(infile, summary->metadata, buffSize);
    read_metadata(infile, summary->metadata, summary->mode.index);
    read_content(infile, &summary->inData, config.numChar);
}

void seek_metadata(FILE *file, MetadataItem items[], int buffSize)
{
    int flag = NOT_METADATA;
    char buff[buffSize];

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
    int flag;               // flag to determine if EOF

    int runningTotal = 0;   // total number of characters
    char *temp = calloc(1, sizeof(char)); // allocation for the content string

    char buff[MAX_CHAR];
    char prev[MAX_CHAR];

    do {
        flag = fscanf(file, "%s", buff);

        if(__check_if_content_end(buff, prev)) {
            break;
        }

        runningTotal += strlen(buff) + 1; // + 1 for the space
        
        if(maxChar != 0 && runningTotal > maxChar)
            break;

        temp = realloc(temp, runningTotal + 1); // + 1 for null byte

        strcat(buff, " ");
        strcat(temp, buff);
        strcpy(prev, buff);
    } while(flag != EOF);

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

bool __check_if_content_end(char buff[], char prev[])
{
    char temp[MAX_CHAR];
    strcpy(temp, prev);
    strcat(temp, buff);

    char *startingToken = CONTENT_END_SIGNIFIER;
    int len = strlen(startingToken);
    temp[len] = '\0';

    return strcmp(startingToken, temp) == 0;
}

