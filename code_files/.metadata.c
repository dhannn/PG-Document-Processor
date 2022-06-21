/*******************************************************************************
 * 
 * FILE             .metadata.c
 * LAST MODIFIED    06-16-2020
 * 
 * DESCRIPTION
 *      This file contains the function implementations involving 
 *      the Metadata structure.
 * 
 ******************************************************************************/

#include "pgdocs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NOT_METADATA -1

const char *METADATA_ITEM_NAMES[] = {
    "Title",
    "Author",
    "Release Date",
    "Language",
    "Produced by",
};

void initialize_metadata(MetadataItem metadataItems[])
{
    for(int i = 0; i < MAX_METADATA; i++) {
        metadataItems[i].data = NULL;
        metadataItems[i].fieldName = METADATA_ITEM_NAMES[i];
    }
}

void set_metadata(MetadataItem items[], int metadataIndex, char *data)
{
    char *temp = calloc(strlen(data) + 1, 1);
    strcpy(temp, data);
    items[metadataIndex].data = temp;
}

int get_metadata_index(MetadataItem metadataItems[], char *fieldName)
{
    int i;
    int flag = NOT_METADATA;
    
    for(i = 0; i < MAX_METADATA && flag == NOT_METADATA; i++) {
        if(strcmp(metadataItems[i].fieldName, fieldName) == 0)
            flag = i;
    }
    
    return flag;
}

void delete_metadata(MetadataItem metadata[])
{
    for(int i = 0; i < MAX_METADATA; i++) {
        if(metadata[i].data != NULL) {
            free(metadata[i].data);
            metadata[i].data = NULL;
        }
    }
}
