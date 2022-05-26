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

#define ESC "\x1b"
#define CLEAR() (printf("%s[2J", ESC))
#define MOVE(row, col) (printf("%s[%d;%df", (ESC), (row), (col)))

#define FMT_BOLD        1
#define FMT_ITALIC      3
#define FMT_UNDERLINE   4

#define COLOR(FG, BG) (printf("%s[%d;%dm", (ESC), (FG), (BG)))
#define FORMAT(FMT) (printf("%s[%dm", (ESC), (FMT)))
#define RESET() (COLOR(0, 0))

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
        "Analyze Document",
        "Configurations"
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

ScreenOption *__initialize_options(int screenIndex);
void __display_header(Screen *screens, int screenIndex);
void __display_options(Screen *screens, int screenIndex);

Screen *initialize_screens()
{
    Screen *screens = malloc(sizeof(Screen) * MAX_SCREENS);
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        const char **headerRow = HEADERS[i];

        screens[i].header = headerRow;
        screens[i].options = __initialize_options(i);
        screens[i].input = "";
    }

    screens[0].options[0].next_screen = &screens[1];
    screens[0].options[1].next_screen = &screens[2];

    return screens;
}

void display_screen(Screen *screen, int screenIndex)
{
    CLEAR();
    __display_header(screen, screenIndex);
    __display_options(screen, screenIndex);
}

ScreenOption *__initialize_options(int screenIndex)
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

void destroy_screens(Screen *screen)
{
    for(int i = 0; i < MAX_SCREENS; i++) {
        free((ScreenOption*)screen[i].options);
    }

    free(screen);
}

void __display_header(Screen *screens, int screenIndex)
{
    int headerWidth = strlen(screens[screenIndex].header[0]) * WSTRLEN_COEFFFICIENT;
    int headerStart = (MAX_WIDTH - headerWidth) / 2;
    const char **header = screens[screenIndex].header;

    for(int row = 0; row < MAX_HEADER_ROW; row++) {
        MOVE(1 + row, headerStart);
        printf("%s", header[row]);
    }
}

void __display_options(Screen *screens, int screenIndex)
{
    ScreenOption *options = screens[screenIndex].options;
    printf("\n\n");

    for(int i = 0; options[i].index != NULL_OPTION; i++) {
        FORMAT(FMT_BOLD);
        printf("\t[%d]", options[i].index + 1);

        RESET();
        printf(" %s\n", options[i].optionName);
    }

    
    RESET();
    FORMAT(FMT_BOLD);
    printf("\n\tPress the number of your choice\n");
    RESET();
    printf("\t>> ");
}
