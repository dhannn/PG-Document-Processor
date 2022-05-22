#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgdocs.h"

#define MAX_WIDTH       85
#define MAX_HEIGHT      35
#define MAX_HEADER_ROW  2

#define MAX_SCREENS     5
#define MAX_OPTIONS     10

#define NULL_OPTION     -1

#define NUM_DEFAULT_OPTIONS     2

#define WSTRLEN_COEFFFICIENT    (0.350649351)

const char *TITLE[] = {
    "█▀█ █▀▀   █▀▄ █▀█ █▀▀ █░█ █▀▄▀█ █▀▀ █▄░█ ▀█▀   █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀ █▀█ █▀█",
    "█▀▀ █▄█   █▄▀ █▄█ █▄▄ █▄█ █░▀░█ ██▄ █░▀█ ░█░   █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█ █▄█ █▀▄"
};

const char *DATA_CLEANER_HEADER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   █▀▀ █░░ █▀▀ ▄▀█ █▄░█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▄▄ █▄▄ ██▄ █▀█ █░▀█ ██▄ █▀▄"
};

const char *DATA_ANALYZER_HEADER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   ▄▀█ █▄░█ ▄▀█ █░░ █▄█ ▀█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▀█ █░▀█ █▀█ █▄▄ ░█░ █▄ ██▄ █▀▄"
};

const char **HEADERS[] = {
    TITLE, 
    DATA_CLEANER_HEADER, 
    DATA_ANALYZER_HEADER, 
    DATA_ANALYZER_HEADER, 
    DATA_ANALYZER_HEADER
};

const char *OPTION_NAME_PER_SCREEN[MAX_SCREENS][MAX_OPTIONS] = {
    {
        "Clean Document", 
        "Analyze Document"
    },
    {
        "To Lowercase", 
        "Remove Special Characters", 
        "Remove Numbers",
        "Clean Whitespaces",
        "Stopwords Removal"
    },
    {
        "Single-File Analysis",
        "Multi-File Analysis"
    },
    {
        "Word Count",
        "N-gram Count",
        "Concordance"
    }, 
    {
        "tf-idf",
        "Document Similarity",
        "Document Clustering"
    }
};

const char *DEFAULT_OPTIONS_NAME[] = {
    "Help",
    "Back"
};

const ScreenOption *initialize_options(int screenIndex);
void destroy_screens();

Screen *initialize_screens()
{
    Screen *screen = malloc(sizeof(Screen) * MAX_SCREENS);
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        const char **headerRow = HEADERS[i];
        screen[i].header = headerRow;
        screen[i].options = initialize_options(i);
    }

    return screen;
}

const ScreenOption *initialize_options(int screenIndex)
{
    ScreenOption *options = malloc(sizeof(ScreenOption) * MAX_OPTIONS);

    int numDefault = 0;

    // TODO: refactor ugly logic
    for(int i = 0; i < MAX_OPTIONS; i++) {
        if(OPTION_NAME_PER_SCREEN[screenIndex][i] != NULL) {
            options[i].index = i;
            options[i].optionName = OPTION_NAME_PER_SCREEN[screenIndex][i];
            options[i].previous_screen = NULL;
            options[i].next_screen = NULL;
            continue;
        }

        if(numDefault < NUM_DEFAULT_OPTIONS) {
            options[i].index = i;
            options[i].optionName = DEFAULT_OPTIONS_NAME[numDefault];
            options[i].previous_screen = NULL;
            options[i]. next_screen = NULL;
            numDefault++;
        } else {
            options[i].index = NULL_OPTION;
            options[i].optionName = NULL;
        }
    }

    return options;
}
