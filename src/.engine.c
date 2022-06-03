#include "pgdocs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HELP_FILE "dat/help.txt"
#define ESC "\x1b"
#define CLEAR() (printf("%s[2J", (ESC)))
#define MOVE(ROW, COL) (printf("%s[%d;%df", (ESC), (ROW), (COL)))


void load_help(ActiveScreen* active, Summary *summary, Config *config)
{
    
}

void return_screen(ActiveScreen* active, Summary *summary, Config *config)
{
    int index = active->current->backIndex;
    
    if(index == EXIT) {
        // TODO: move this code snippet to a function in display.c
        MOVE(1, 1);
        CLEAR();
        fflush(stdin);
        printf("Press ENTER to exit.");
        scanf("%*c");
        exit(EXIT_SUCCESS);
    }

    go_to_screen(active, index);
}

void get_filename_for_processing(
    ActiveScreen* active, Summary *summary, Config *config)
{
    int mode = active->nInput - 1;
    set_mode(summary, mode);

    go_to_screen(active, ENTER_FILENAME_MENU);
}

void choose_option(ActiveScreen* active, Summary *summary, Config *config)
{
    char *filename = active->strInput;
    // TODO: finish and call set_infile

    int mode = summary->mode.index;
    int screen = MAIN_MENU;

    switch(mode) {
        case CLEAN:
            screen = CLEAN_DOCUMENT_MENU;
            break;
        case ANALYZE_SINGLE:
            screen = SINGLE_ANALYZE_DOCUMENT_MENU;
            break;
        case ANALYZE_MULTI:
            screen = MULTI_ANALYZE_DOCUMENT_MENU;
            break;
    }

    go_to_screen(active, screen);
}

void do_processing(ActiveScreen* active, Summary *summary, Config *config)
{
    summary->mode.clean_or_analyze();
}

bool check_if_exit(Screen screens[], ActiveScreen *active)
{
    Screen *mainScreen = &screens[MAIN_MENU];
    return active->current == mainScreen && active->nInput == mainScreen->backIndex;
}
