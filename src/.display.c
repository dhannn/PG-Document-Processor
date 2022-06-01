#include "pgdocs.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    CONFIG, 
    MAIN_MENU,
    ENTER_FILENAME_MENU,
    CLEAN_DOCUMENT_MENU,
    SINGLE_ANALYZE_DOCUMENT_MENU,
    MULTI_ANALYZE_DOCUMENT_MENU,
    ENTER_N_MENU,
    ENTER_KEYWORD_MENU,
    ENTER_NUMBER_CLUSTERS_MENU, 
    ENTER_PATH_MENU,
    ENTER_NUMBER_CHAR_MENU,
    ENTER_MULTISELECT_BOOL_MENU,
} ScreenTag;

/* -------------------------------------------------------------------------- */
/*                            CONSTANTS FOR HEADER                            */
/* -------------------------------------------------------------------------- */

// no more than 80 characters per line rule is overruled in this constant
char *TITLE_HEADER[] = {
    "█▀█ █▀▀   █▀▄ █▀█ █▀▀ █░█ █▀▄▀█ █▀▀ █▄░█ ▀█▀   █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀ █▀█ █▀█",
    "█▀▀ █▄█   █▄▀ █▄█ █▄▄ █▄█ █░▀░█ ██▄ █░▀█ ░█░   █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█ █▄█ █▀▄"
};

char *DATA_CLEANER_HEADER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   █▀▀ █░░ █▀▀ ▄▀█ █▄░█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▄▄ █▄▄ ██▄ █▀█ █░▀█ ██▄ █▀▄"
};

char *DATA_ANALYZER_HEADER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   ▄▀█ █▄░█ ▄▀█ █░░ █▄█ ▀█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▀█ █░▀█ █▀█ █▄▄ ░█░ █▄ ██▄ █▀▄"
};

char *CONFIG_HEADER[] = {
    "█▀▀ █▀█ █▄░█ █▀▀ █ █▀▀",
    "█▄▄ █▄█ █░▀█ █▀░ █ █▄█"
};

char **HEADERS[] = {
    CONFIG_HEADER,
    TITLE_HEADER,
    TITLE_HEADER,
    DATA_CLEANER_HEADER,
    DATA_ANALYZER_HEADER,
    DATA_ANALYZER_HEADER,
    TITLE_HEADER,
    TITLE_HEADER,
    TITLE_HEADER,
    CONFIG_HEADER,
    CONFIG_HEADER,
    CONFIG_HEADER
};

/* -------------------------------------------------------------------------- */
/*                            CONSTANTS FOR PROMPTS                           */
/* -------------------------------------------------------------------------- */

char *PROMPTS[] = {
    "Enter the number of the configuration option you want to change",
    "Enter the number of your choice",
    "Enter a valid filename of your chosen document",
    "Enter the number of your choice",
    "Enter the number of your choice",
    "Enter the number of your choice",
    "Enter the value of N",
    "Enter a keyword to be searched",
    "Enter the number of clusters to be analyzed",
    "Enter a valid path",
    "Enter the number of characters you want to read (enter 0 if all)",
    "Enter 1 if you want multiselect on; 0 if otherwise"
};

/* -------------------------------------------------------------------------- */
/*                         CONSTANTS FOR OPTION NAMES                         */
/* -------------------------------------------------------------------------- */

// If NULL, it means that the screen is not a multi-option input
char *OPTION_NAMES[][MAX_OPTIONS] = {
    {
        "Default path for raw documents",
        "Default path for cleaned documents",
        "Default path for analyses",
        "Default number for characters to read",
        "Multi-select?"
    }, 
    {
        "Clean document",
        "Analyze document (Single-file)",
        "Analyze document (Multi-file)",
        "Config"
    }, 
    {""},
    {
        "To lowercase",
        "Remove special characters",
        "Remove numbers",
        "Clean whitespaces",
        "Remove stopwords"
    },
    {
        "Word count",
        "N-gram count",
        "Concordance"
    },
    {
        "tf-idf",
        "Document clustering"
    },
    {""},
    {""},
    {""},
    {""},
    {""},
    {""}
};

char *DEFAULT_OPTION_NAMES[MAX_OPTIONS] = {
    "Help",
    "Back"
};

void __extract_options(Screen *screens, int index);
void __extract_back_index(Screen *screens, int index);

void initialize_screens(Screen *screens)
{
    for(int i = 0; i < MAX_SCREENS; i++) {
        screens[i].header = HEADERS[i];
        screens[i].prompt = PROMPTS[i];
        __extract_options(screens, i);
        __extract_back_index(screens, i);
    }
}

void __extract_options(Screen *screens, int index)
{
    Screen *current = &screens[index];
    int numDefaultOpts = 0;

    for(int i = 0; i < MAX_OPTIONS; i++) {
        ScreenOption *opt = &current->options[i];

        if(OPTION_NAMES[index][i] != NULL) {
            strcpy(opt->optionName, OPTION_NAMES[index][i]);
            continue;
        }

        // TODO: refactor ugly logic
        if(DEFAULT_OPTION_NAMES[numDefaultOpts] != NULL && strcmp(OPTION_NAMES[index][0], "") != 0) {
            strcpy(opt->optionName, DEFAULT_OPTION_NAMES[numDefaultOpts]);
            numDefaultOpts++;
        } else 
            strcpy(opt->optionName, "");
    }
}

void __extract_back_index(Screen *screens, int index)
{
    Screen *current = &screens[index];

    for(int i = 0; i < MAX_OPTIONS; i++) {
        ScreenOption *opt = &current->options[i];

        if(strcmp(opt->optionName, "Back") == 0) {
            current->backIndex = i;
            break;
        }
    }
}

void go_to_screen(Screen screens[], ActiveScreen *active)
{
    int option = active->nInput - 1;
    
    active->screen = &screens[option];
}

void get_int(ActiveScreen *active)
{
    scanf ("%d", &active->nInput);
}

void get_str(ActiveScreen *active)
{
    scanf ("%s", active->strInput);
}

void print_metadata(char *metadataName[], char *metadata[], int maxMetadata)
{
    printf ("************************************************\n");
		
    for (int i = 0; i < maxMetadata; i++){
        printf ("%s: %s\n", metadataName[i], metadata[i]);
     }

    printf ("************************************************\n");
}
