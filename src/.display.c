#include "pgdocs.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SCREENS 12
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
    COMMAND_OPTIONS,
    DO_COMMAND
} ScreenTag;

/* -------------------------------------------------------------------------- */
/*                            CONSTANTS FOR HEADER                            */
/* -------------------------------------------------------------------------- */

// no more than 80 characters per line rule is overruled in this constant
const char *TITLE_HEADER[] = {
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

const char *CONFIG_HEADER[] = {
    "█▀▀ █▀█ █▄░█ █▀▀ █ █▀▀",
    "█▄▄ █▄█ █░▀█ █▀░ █ █▄█"
};

const char **HEADERS[] = {
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

const *PROMPTS[] = {
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
const char *OPTION_NAMES[] = {
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
    NULL,
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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

ScreenTag *RESULTING_SCREENTAG_PER_OPTION[] = {
    {
        ENTER_PATH_MENU,
        ENTER_PATH_MENU,
        ENTER_PATH_MENU,
        ENTER_NUMBER_CHAR_MENU,
        ENTER_MULTISELECT_BOOL_MENU
    },
    {
        CLEAN_DOCUMENT_MENU,
        SINGLE_ANALYZE_DOCUMENT_MENU,
        MULTI_ANALYZE_DOCUMENT_MENU
    }, 
    {
        COMMAND_OPTIONS
    },
    {
        DO_COMMAND,
        DO_COMMAND,
        DO_COMMAND,
        DO_COMMAND,
        DO_COMMAND
    }, 
    {
        DO_COMMAND,
        ENTER_N_MENU,
        ENTER_KEYWORD_MENU
    }, 
    {
        DO_COMMAND,
        ENTER_NUMBER_CLUSTERS_MENU
    },
    {
        DO_COMMAND
    },
    {
        ENTER_N_MENU
    },
    {
        DO_COMMAND
    }, 
    {
        CONFIG
    },
    {
        CONFIG
    },
    {
        CONFIG
    },
    NULL,
    NULL
};

ScreenOption *__extract_options(Screen *screens, int index);

void initialize_screens(Screen *screens)
{
    for(int i = 0; i < MAX_SCREENS; i++) {
        screens->header = HEADERS[i];
        screens->options = __extract_options(screens, i);
    }
}

ScreenOption *__extract_options(Screen *screens, int index)
{
    Screen *current = &screens[index];
    ScreenOption *opt = &current->options;
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        strcpy(current->options->optionName, OPTION_NAMES[index]);
    }
}
