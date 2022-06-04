#include "pgdocs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HELP_FILE "dat/help.txt"
#define CONFIG_FILE "dat/config"
#define ESC "\x1b"
#define CLEAR() (printf("%s[2J", (ESC)))
#define MOVE(ROW, COL) (printf("%s[%d;%df", (ESC), (ROW), (COL)))

void initialize_config(ActiveScreen *active, Config *config)
{
    if(check_config_initialized()) {
        load_config(config);
        return;
    }

    const ScreenTag CONFIG_SCREENS[] = {
        ENTER_RAW_PATH_MENU, 
        ENTER_CLEANED_PATH_MENU,
        ENTER_ANALYSIS_PATH_MENU,
        ENTER_NUMBER_CHAR_MENU,
        ENTER_MULTISELECT_BOOL_MENU
    };
    
    FILE *configFile = fopen("dat/config", "w");
    fclose(configFile);

    for(int i = 0; i < 5; i++) {
        
        int index = CONFIG_SCREENS[i];
        
        go_to_screen(active, index);
        display_screen(active, NULL);
        active->current->get_input(active);

        if(i < 3) {
            set_config_path(config, i, active->strInput);
        } else {
            set_config_int(config, i, active->nInput);
        }
    }
}

void reset_config(ActiveScreen *active, Summary *summary, Config *config)
{
    remove(CONFIG_FILE);
}

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
    set_infile(summary, *config, filename);

    int mode = summary->mode.index;
    int screen = MAIN_MENU;
    
    initialize_metadata(summary->metadata);
    read_file(summary, *config);
    summary->tokenList = tokenize_string(summary->inData, summary->mode.index == CLEAN);

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
    set_options(summary, *config, active->nInput);
    execute_summary(summary, *config);
}

bool check_if_exit(Screen screens[], ActiveScreen *active)
{
    Screen *mainScreen = &screens[MAIN_MENU];
    return active->current == mainScreen && active->nInput == mainScreen->backIndex;
}
