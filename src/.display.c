#include "pgdocs.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_WIDTH       85
#define MAX_HEIGHT      35
#define WSTRLEN_COEFFFICIENT    (0.350649351)

#define ESC "\x1b"
#define CLEAR() (printf("%s[2J", (ESC)))
#define MOVE(ROW, COL) (printf("%s[%d;%df", (ESC), (ROW), (COL)))

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
    "Enter a valid path for the raw document files",
    "Enter a valid path for the cleaned document files",
    "Enter a valid path for the analysis files",
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
        "Multi-select?",
        "Reset config"
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
    {""},
    {""},
    {""}
};

char *DEFAULT_OPTION_NAMES[MAX_OPTIONS] = {
    "Help",
    "Back"
};

int BACK_INDICES[] = {
    MAIN_MENU, 
    EXIT,
    -1,
    MAIN_MENU,
    MAIN_MENU,
    MAIN_MENU,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1
};

/* -------------------------------------------------------------------------- */
/*                 CONSTANTS FOR FUNCTION POINTERS PER OPTION                 */
/* -------------------------------------------------------------------------- */

void (*GET_INPUT_FUNCTIONS[])(ActiveScreen*) = {
    get_int,
    get_int,
    get_str,
    get_int,
    get_int,
    get_int,
    get_int,
    get_str,
    get_int,
    get_str,
    get_str,
    get_str,
    get_int,
    get_int
};

void (*DO_DEFAULT_OPTION[])(ActiveScreen*, Summary*, Config*) = {
    load_help,
    return_screen
};


void (*DO_OPTION[][MAX_OPTIONS])(ActiveScreen*, Summary*, Config*) = {
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        reset_config
    },
    {
        get_filename_for_processing,
        get_filename_for_processing,
        get_filename_for_processing
    },
    {
        choose_option
    },
    {
        do_processing,
        do_processing,
        do_processing,
        do_processing,
        do_processing
    },
    {
        do_processing,
        do_processing,
        do_processing
    },
    {
        do_processing,
        do_processing
    }
};

void __extract_options(Screen *screens, int index);
void __extract_back_index(Screen *screens, int index);

void initialize_screens(ActiveScreen *screen)
{
    Screen *screens = screen->screens;
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        screens[i].header = HEADERS[i];
        screens[i].prompt = PROMPTS[i];
        screens[i].get_input = GET_INPUT_FUNCTIONS[i];
        screens[i].backIndex = BACK_INDICES[i];
        __extract_options(screens, i);
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
            opt->do_option = DO_OPTION[index][i];
            continue;
        }

        // TODO: refactor ugly logic
        if(DEFAULT_OPTION_NAMES[numDefaultOpts] != NULL && strcmp(OPTION_NAMES[index][0], "") != 0) {
            strcpy(opt->optionName, DEFAULT_OPTION_NAMES[numDefaultOpts]);
            opt->do_option = DO_DEFAULT_OPTION[numDefaultOpts];
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

int __get_starting_cell(int strlen)
{
    strlen *= WSTRLEN_COEFFFICIENT;
    int start = (MAX_WIDTH - strlen) / 2;

    return start;
}

void display_screen(ActiveScreen *active, Summary *summary)
{
    // CLEAR();

    printf("\n");
    for (int i = 0; i < 2; i++) {
        char *headerRow = active->current->header[i];
        int startingCell = __get_starting_cell(strlen(headerRow));

        // MOVE(2 + i, startingCell);
        printf("%s\n", headerRow);
    }

    printf("\n");

    if(active->current == &active->screens[CLEAN_DOCUMENT_MENU] ||
        active->current == &active->screens[SINGLE_ANALYZE_DOCUMENT_MENU] ||
        active->current == &active->screens[MULTI_ANALYZE_DOCUMENT_MENU]
    ) {
        char metadata[MAX_METADATA][MAX_CHAR];
        char metadataItem[MAX_METADATA][MAX_CHAR];

        for(int i = 0; i < MAX_METADATA; i++) {
            strcpy(metadata[i], summary->metadata[i].name);
            
            if(summary->metadata[i].data != NULL) 
                strcpy(metadataItem[i], summary->metadata[i].data);
            else
                strcpy(metadataItem[i], "");
        }

        print_metadata(metadata, metadataItem, MAX_METADATA);
        printf("\n");
    }

    for (int i = 0; i < MAX_OPTIONS; i++){
        if (strcmp(active->current->options[i].optionName, "") != 0)
            printf("\t[%d] %s\n", i + 1, active->current->options[i].optionName);
    }

    printf("\n\t%s\n", active->current->prompt);
    printf("\t>> ");
}


void go_to_screen(ActiveScreen *active, int index)
{   
    Screen *screens = active->screens;
    active->current = &screens[index];
}

void get_int(ActiveScreen *active)
{
    scanf ("%d", &active->nInput);
}

void get_str(ActiveScreen *active)
{
    scanf ("%s", active->strInput);
}

void print_metadata(char metadataName[][MAX_CHAR], char metadata[][MAX_CHAR], int maxMetadata)
{
    printf ("*************************************************************************************\n\n");
		
    for (int i = 0; i < maxMetadata; i++){
        if(strcmp(metadata[i], "") != 0)
            printf ("\t%s: %s\n", metadataName[i], metadata[i]);
     }

    printf ("\n*************************************************************************************\n");
}


