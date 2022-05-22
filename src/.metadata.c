#include "pgdocs.h"
#include "deps/internals.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NOT_METADATA -1

char *_get_starting_token(Mode mode);

const char *METADATA_ITEM_NAMES[] = {
    "Title",
    "Author",
    "Release Date",
    "Language",
    "Produced by",
};

Metadata initialize_metadata()
{
    Metadata metadata;

    for(int i = 0; i < MAX_METADATA_ITEMS; i++) {
        metadata.metadataItems[i].data = NULL;
        metadata.metadataItems[i].name = METADATA_ITEM_NAMES[i];
    }

    return metadata;
}

void set_metadata(MetadataItem items[], int metadataIndex, char *data)
{
    char *temp = malloc(strlen(data) + 1);
    strcpy(temp, data);
    items[metadataIndex].data = temp;
}

int get_metadata_index(MetadataItem metadataItems[], char *str)
{
    int i;
    int flag = NOT_METADATA;
    
    for(i = 0; i < MAX_METADATA_ITEMS && flag == NOT_METADATA; i++) {
        if(strcmp(metadataItems[i].name, str) == 0)
            flag = i;
    }
    
    return flag;
}

void delete_metadata(Metadata *metadata)
{
    for(int i = 0; i < MAX_METADATA_ITEMS; i++) {
        if(metadata->metadataItems[i].data != NULL)
            free(metadata->metadataItems[i].data);
    }
}
