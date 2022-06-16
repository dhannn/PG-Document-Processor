/*******************************************************************************
 * 
 * FILE             .display.c
 * LAST MODIFIED    06-17-2022
 * 
 * DESCRIPTION
 *      This file contains the function implementations of display functions
 * 
 ******************************************************************************/

#include "pgdocs.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_WIDTH       85
#define MAX_HEIGHT      35
#define WSTRLEN_COEFFFICIENT    (0.350649351)

// ansi escape codes
#define ESC "\x1b"

#ifdef DEBUG
#define CLEAR() (ESC)
#define MOVE(ROW, COL) (ESC)
#else
#define CLEAR() (printf("%s[2J", (ESC)))
#define MOVE(ROW, COL) (printf("%s[%d;%df", (ESC), (ROW), (COL)))
#endif
#define FMT(X) (printf("%s[%dm", (ESC), (X)))
#define HIDE_CURS() (printf("%s[?25l", (ESC)))
#define SHOW_CURS() (printf("%s[?25h", (ESC)))
#define COLOR(X, Y) (printf("%s[%d;%dm", (ESC), (X), (Y)))

enum FORMAT {
    BOLD = 1,
    DIM,
    ITALIC,
    UNDERLINE
};

enum COLORS{
    RESET_COLOR = 0,
    BLACK_FG = 30,
    RED_FG,
    GREEN_FG,
    YELLOW_FG,
    BLUE_FG,
    MAGENTA_FG,
    CYAN_FG,
    WHITE_FG,
    DEFAULT_FG = 39,
    BLACK_BG,
    RED_BG,
    GREEN_BG,
    YELLOW_BG,
    BLUE_BG,
    MAGENTA_BG,
    CYAN_BG,
    WHITE_BG,
    DEFAULT_BG
};

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
    CONFIG_HEADER,
    TITLE_HEADER
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
    "Enter 1 if you want multiselect on; 0 if otherwise",
    "Enter a valid filename for your output"
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
        "Default maximum number of documents to be read in corpus"
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
        "Remove stopwords",
        "All"
    },
    {
        "Word count",
        "N-gram count",
        "Concordance"
    },
    {
        "tf-idf",
        "Document similarity",
        "Document clustering"
    },
    {""},
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
    -1,
    -1
};

/* -------------------------------------------------------------------------- */
/*                 CONSTANTS FOR FUNCTION POINTERS PER OPTION                 */
/* -------------------------------------------------------------------------- */

void (*GET_INPUT_FUNCTIONS[])(ActiveScreen*) = {
    get_choice,
    get_choice,
    get_str,
    get_choice,
    get_choice,
    get_choice,
    get_int,
    get_str,
    get_int,
    get_str,
    get_str,
    get_str,
    get_int,
    get_int,
    get_str
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
        do_chosen_mode,
        do_chosen_mode,
        do_chosen_mode
    },
    {
        NULL
    },
    {
        do_clean_options,
        do_clean_options,
        do_clean_options,
        do_clean_options,
        do_clean_options,
        do_default_option
    },
    {
        do_default_option,
        do_ngram_count,
        do_concordance
    },
    {
        do_default_option,
        do_doc_similarity
    }, 
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        NULL
    },
    {
        save_results
    }, 
    {
        NULL
    }
};

void __validate_screen_option(ActiveScreen *active);
void __extract_options(Screen *screens, int index);
void __extract_back_index(Screen *screens, int index);

void __validate_choice(ActiveScreen *active, Summary *summary, int index)
{
    int maxNumOptions = active->current->numOptions;

    while(active->choice <= 0 || active->choice > maxNumOptions) {
        display_error(ERR_INVALID_CHOICE);
        go_to_screen(active, summary, index);
    }
}

void initialize_screens(ActiveScreen *screen)
{
    Screen *screens = screen->screens;
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        screens[i].index = i;
        screens[i].header = HEADERS[i];
        screens[i].prompt = PROMPTS[i];
        screens[i].get_input = GET_INPUT_FUNCTIONS[i];
        screens[i].backIndex = BACK_INDICES[i];

        screens[i].numOptions = 0;
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
            current->numOptions++;
            continue;
        }

        // TODO: refactor ugly logic
        if(DEFAULT_OPTION_NAMES[numDefaultOpts] != NULL && strcmp(OPTION_NAMES[index][0], "") != 0) {
            strcpy(opt->optionName, DEFAULT_OPTION_NAMES[numDefaultOpts]);
            opt->do_option = DO_DEFAULT_OPTION[numDefaultOpts];
            numDefaultOpts++;
            current->numOptions++;
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
    CLEAR();
    MOVE(1, 1);
    SHOW_CURS();

    printf("\n");
    for (int i = 0; i < 2; i++) {
        char *headerRow = active->current->header[i];
        int startingCell = __get_starting_cell(strlen(headerRow));

        MOVE(2 + i, startingCell);
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
            strcpy(metadata[i], summary->metadata[i].fieldName);
            
            if(summary->metadata[i].data != NULL) 
                strcpy(metadataItem[i], summary->metadata[i].data);
            else
                strcpy(metadataItem[i], "");
        }

        print_metadata(metadata, metadataItem);
        printf("\n");
    }

    for (int i = 0; i < MAX_OPTIONS; i++){
        if (strcmp(active->current->options[i].optionName, "") != 0) {
            FMT(YELLOW_FG);
            printf("\t[");
            printf("%d]", i + 1);
            COLOR(RESET_COLOR, RESET_COLOR);
            printf(" %s\n", active->current->options[i].optionName);
        }
    }

    printf("\n\t%s\n", active->current->prompt);
    printf("\t>> ");
}


void go_to_screen(ActiveScreen *active, Summary *summary, int index)
{   
    Screen *screens = active->screens;
    active->current = &screens[index];

    display_screen(active, summary);
    active->current->get_input(active);

    if(active->current->get_input == get_choice) {
        __validate_choice(active, summary, index);
    }
}

void get_choice(ActiveScreen *active)
{
    FMT(YELLOW_FG);
    scanf ("%d", &active->choice);
    COLOR(RESET_COLOR, RESET_COLOR);
}

void get_int(ActiveScreen *active)
{
    FMT(YELLOW_FG);
    scanf ("%d", &active->nInput);
    COLOR(RESET_COLOR, RESET_COLOR);
}

void get_str(ActiveScreen *active)
{
    FMT(YELLOW_FG);
    scanf ("%s", active->strInput);
    COLOR(RESET_COLOR, RESET_COLOR);
}

void print_metadata(char fieldName[][MAX_CHAR], char data[][MAX_CHAR])
{
    FMT(CYAN_FG);
    printf ("*************************************************************************************\n\n");
		
    for (int i = 0; i < MAX_METADATA; i++){
        if(strcmp(data[i], "") != 0) { 
            FMT(RESET_COLOR); FMT(CYAN_FG);
            printf ("\t%s:", fieldName[i]);
            FMT(RESET_COLOR); FMT(DIM);
            FMT(BOLD);
            printf (" %s\n", data[i]);
        }
     }

    FMT(RESET_COLOR);
    FMT(CYAN_FG);
    printf ("\n*************************************************************************************\n");
    FMT(RESET_COLOR);
}

void print_token_frequency(Summary *summary)
{
    FILE *outfile = summary->outfile;
    char *results = summary->outData;
    char buff[MAX_CHAR];
    int len = 0;
    int linesRead = 0;
    int flag = sscanf(results + len, "%[^\n]\ns", buff);
    
    CLEAR();
    MOVE(1, 1);

    while(flag != EOF) {
        if(linesRead < 10)
            fprintf(stdout, "%s\n", buff);
        
        fprintf(outfile , "%s\n", buff);
        
        len += strlen(buff) + 1; // the "+ 1" accounts for the newline
        linesRead++;
        
        flag = sscanf(results + len, "%[^\n]\ns", buff);
    }
}

void print_concordance(Summary *summary)
{
    FILE *outfile = summary->outfile; 
    char *results = summary->outData;
    int n = summary->addOpts.i[0];

    int flag;
    int length = 0;
    int numWords = 0;
    int linesRead = 0;
    char buff[MAX_CHAR] = "";

    MOVE(1, 1);
    do {
        flag = sscanf(results + length, "%s", buff);
        numWords++;

        fprintf(outfile, "%s ", buff);

        if(linesRead < 10) {
            if(numWords % (2 * n + 1) == n + 1) {
                FMT(BOLD);
                FMT(CYAN_FG);
            }
            
            fprintf(stdout, "%s", buff);

            FMT(RESET_COLOR);
            fprintf(stdout, " ");
        
            if((numWords % (2 * n + 1)) == 0)
                fprintf(stdout, "\n");
        }

        if((numWords % (2 * n + 1)) == 0) {
            fprintf(outfile, "\n");
            
            linesRead++;
        }
        
        length += strlen(buff) + 1;
    } while(flag != EOF);
}

void update_reading(int characters, int words)
{
    if(words % 1024 == 0) {
        HIDE_CURS();
        MOVE(1, 1);
        printf("Reading:\t\t");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d\t", characters);
        FMT(RESET_COLOR);
        printf("characters/s\n");

        FMT(BOLD);
        FMT(CYAN_FG);
        printf("\t\t\t%d\t", words);
        FMT(RESET_COLOR);
        printf("words/s\n");

        MOVE(1, 1);
    }
}

void update_tokenizing(char *current, int words)
{
    if(words % 1024 == 0) {
        HIDE_CURS();
        MOVE(4, 1);
        
        printf("\x1b[2K");
        MOVE(4, 1);
        FMT(RESET_COLOR);
        printf("Tokenizing data:\t");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d\t", words);
        FMT(RESET_COLOR);
        printf("tokens/s done\n");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("\x1b[2K\t\t\t%s\t", current);
        FMT(RESET_COLOR);
        
        MOVE(4, 1);
    }
}

void update_processing(int progress, int max)
{
    HIDE_CURS();
    if(progress % 1024 == 0) {
        MOVE(1, 1);
        FMT(RESET_COLOR);
        printf("Processing:\t");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d", progress);
        FMT(RESET_COLOR);
        printf(" out of ");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d", max);
        FMT(RESET_COLOR);
        printf(" tokens/s done\n");
        MOVE(1, 1);
    }
}

void update_reporting(int progress, int max)
{
    if(progress % 1024 == 0) {
        HIDE_CURS();
        MOVE(1, 1);
        FMT(RESET_COLOR);
        printf("Reporting:\t");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d", progress);
        FMT(RESET_COLOR);
        HIDE_CURS();
        printf(" out of ");
        FMT(BOLD);
        FMT(CYAN_FG);
        printf("%d", max);
        FMT(RESET_COLOR);
        HIDE_CURS();
        printf(" tokens/s done\n");
        MOVE(1, 1);
    }
}

void print_cleaned(Summary *summary)
{
    FILE *outfile = summary->outfile; 
    MetadataItem *metadata = summary->metadata; 
    char *results = summary->outData;

	for(int i = 0; i < MAX_METADATA; i++) {
		MetadataItem currentMetadata = metadata[i];
		
		if(currentMetadata.data != NULL) {
			fprint_metadata_item(
				outfile, 
				currentMetadata.fieldName, 
				currentMetadata.data
			);
		}
	}
    
	fprintf(outfile, "Content:\n");

    fprintf(outfile, "%s\n", results);

    CLEAR();
    MOVE(1, 1);

    HIDE_CURS();
    fprintf(stdout, "Finished cleaning! \nPress ENTER key to return to main menu.");
}

void display_error(ErrorCode errorCode)
{
    char *errorMessage[] = {
        "File not found",
        "Invalid choice",
        "Invalid value of N",
        "Keyword not found in text",
        "Memory allocation failed. No memory left"
    };

    char *errorSuggestion[] = {
        "Try entering a file that exists or re-configure paths in dat/config",
        "Try entering a valid choice",
        "Try entering a number from 1 to 4",
        "Try entering another keyword",
        "Try limiting the number of characters to be read in dat/config"
    };

    CLEAR();
    MOVE(1, 1);
    FMT(BOLD); FMT(RED_FG);
    fprintf(stderr, "ERROR");

    FMT(RESET_COLOR);
    fprintf(stderr, "\t\t");
    fprintf(stderr, "%s!\n", errorMessage[errorCode]);

    FMT(RESET_COLOR);
    fprintf(stderr, "\t\t");
    FMT(YELLOW_FG);
    fprintf(stderr, "%s.", errorSuggestion[errorCode]);

    HIDE_CURS();
    fflush(stdin);
    scanf("%*c");

    SHOW_CURS();
    FMT(RESET_COLOR);
}

void restart_screen()
{
    MOVE(1, 1);
    CLEAR();  
}