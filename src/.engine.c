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
            set_config_int(config, i, active->choice);
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
        return;
    }

    go_to_screen(active, index);
}

void get_filename(
    ActiveScreen* active, Summary *summary, Config *config)
{
    int mode = active->choice - 1;
    set_mode(summary, mode);

    go_to_screen(active, ENTER_INPUT_FILE_MENU);
}

void choose_option(ActiveScreen* active, Summary *summary, Config *config)
{
    char *filename = active->strInput;
    strcpy(summary->infilename, filename);
    set_infile(summary, *config, filename);

    if(summary->infile == NULL) {
        display_error(ERR_FILE_NOT_FOUND);
        go_to_screen(active, ENTER_INPUT_FILE_MENU);

        return;
    }

    int mode = summary->mode.index;
    int screen = MAIN_MENU;
    
    initialize_metadata(summary->metadata);
    
    CLEAR();
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
    set_option(summary, *config, active->choice - 1);

    if(active->current->get_input == get_str) {
        set_add_str(summary, active->strInput);
    } else if(active->current->get_input == get_int) {
        set_add_int(summary, active->nInput);
    }

    if(summary->mode.commands[summary->option].addIntNeeded - summary->mode.commands[summary->option].usedAddInt != 0 || summary->mode.commands[summary->option].addStrNeeded - summary->mode.commands[summary->option].usedAddStr != 0) {
        get_add_opts(active, summary, config);
        return;
    }
    
    CLEAR();
    execute_summary(summary);
    
    if(summary->mode.index == CLEAN)
        go_to_screen(active, ENTER_OUTPUT_FILE_MENU);
    else {
        char filename[MAX_CHAR] = "";
        int option = summary->option;
        char *suffix = summary->mode.commands[option].fileSuffix;
        char *original = strtok(summary->infilename, ".");
        
        strcpy(filename, original);
        strcat(filename, "_");
        strcat(filename, suffix);
        strcat(filename, ".txt");

        strcpy(active->strInput, filename);

        save_results(active, summary, config);
    }
}

void get_add_opts(ActiveScreen* active, Summary *summary, Config *config) 
{
    ScreenTag screen;
    int chosenOption = active->choice - 1;
    set_option(summary, *config, active->choice);
    ModeIndex mode = summary->mode.index;

    ScreenTag addOptsScreen[3][MAX_OPTIONS][MAX_ADD_OPTS] = {
        {
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_OUTPUT_FILE_MENU}
        },
        {
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_N_MENU},
            {ENTER_KEYWORD_MENU, ENTER_N_MENU}
        },
        {
            {ENTER_OUTPUT_FILE_MENU},
            {ENTER_INPUT_FILE_MENU},
            {ENTER_NUMBER_CLUSTERS_MENU}
        }
    };

    int usedInt = summary->mode.commands[chosenOption].usedAddStr;
    screen = addOptsScreen[mode][chosenOption][usedInt];
    go_to_screen(active, screen);
}

void save_results(ActiveScreen* active, Summary *summary, Config *config)
{
    set_outfile(summary, *config, active->strInput);
    
    summary->mode.commands[summary->option].print_results(summary);
    
    fflush(stdin);
    scanf("%*c");
    destroy_summary(summary);

    active->choice = -1;
    go_to_screen(active, MAIN_MENU);
}

bool check_if_exit(Screen screens[], ActiveScreen *active)
{
    Screen *mainScreen = &screens[MAIN_MENU];
    return active->current == mainScreen && active->choice == 6;
}
