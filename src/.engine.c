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

void __validate_file(ActiveScreen *active, Summary *summary, Config config);

void __validate_file(ActiveScreen *active, Summary *summary, Config config)
{
    while(summary->infile == NULL) {
        display_error(ERR_FILE_NOT_FOUND);

        go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);
        set_infile(summary, config, active->strInput);
    }
}

void do_clean(ActiveScreen *active, Summary *summary, Config *config)
{
    int mode = active->choice - 1;
    set_mode(summary, mode);

    go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);    
    set_infile(summary, *config, active->strInput);

    __validate_file(active, summary, *config);
    
    CLEAR();
    MOVE(1, 1);

    initialize_metadata(summary->metadata);
    read_file(summary, *config);
    summary->tokenList = tokenize_string(summary->inData, true);

    go_to_screen(active, summary, CLEAN_DOCUMENT_MENU);
}

void do_s_analyze(ActiveScreen *active, Summary *summary, Config *config) 
{
    int mode = active->choice - 1;
    set_mode(summary, mode);

    go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);
    set_infile(summary, *config, active->strInput);

    __validate_file(active, summary, *config);
    
    CLEAR();
    MOVE(1, 1);

    initialize_metadata(summary->metadata);
    read_file(summary, *config);
    summary->tokenList = tokenize_string(summary->inData, false);

    go_to_screen(active, summary, SINGLE_ANALYZE_DOCUMENT_MENU);
}

void do_m_analyze(ActiveScreen *active, Summary *summary, Config *config) 
{
    int mode = active->choice - 1;
    set_mode(summary, mode);

    go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);
    set_infile(summary, *config, active->strInput);

    __validate_file(active, summary, *config);
    
    CLEAR();
    MOVE(1, 1);

    initialize_metadata(summary->metadata);
    read_file(summary, *config);
    summary->tokenList = tokenize_string(summary->inData, false);

    go_to_screen(active, summary, MULTI_ANALYZE_DOCUMENT_MENU);
}

void do_clean_options(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    CLEAR();
    execute_summary(summary);
    
    char in; 
    CLEAR();
    MOVE(1, 1);
    printf("Continue cleaning? [Y or N]: ");
    scanf(" %c", &in);

    while(in == 'Y' || in == 'y') {
        go_to_screen(active, summary, CLEAN_DOCUMENT_MENU);
        
        set_option(summary, *config, choice);
        execute_summary(summary);

        CLEAR();
        MOVE(1, 1);
        printf("Continue cleaning? [Y or N]: ");
        scanf(" %c", &in);
    }

    go_to_screen(active, summary, ENTER_OUTPUT_FILE_MENU);
    set_outfile(summary, *config, active->strInput);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_clean_all(ActiveScreen *active, Summary *summary, Config *config)
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    CLEAR();
    execute_summary(summary);

    go_to_screen(active, summary, ENTER_OUTPUT_FILE_MENU);
    set_outfile(summary, *config, active->strInput);
    
    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_word_count(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    CLEAR();
    MOVE(1, 1);
    execute_summary(summary);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_ngram_count(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    go_to_screen(active, summary, ENTER_N_MENU);

    // TODO: finish __validate_n
    set_add_int(summary, active->nInput);

    CLEAR();
    MOVE(1, 1);
    execute_summary(summary);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_concordance(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    go_to_screen(active, summary, ENTER_KEYWORD_MENU);

    // TODO: finish __validate_keyword
    set_add_str(summary, active->strInput);

    go_to_screen(active, summary, ENTER_N_MENU);
    display_screen(active, summary);
    get_int(active);
    set_add_int(summary, active->nInput);

    CLEAR();
    MOVE(1, 1);
    execute_summary(summary);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_tfidf(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    CLEAR();
    MOVE(1, 1);
    execute_summary(summary);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_doc_similarity(ActiveScreen *active, Summary *summary, Config *config) 
{
    
}


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
        
        go_to_screen(active, NULL, index);
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

    go_to_screen(active, summary, index);
}

void save_results(ActiveScreen* active, Summary *summary, Config *config)
{    
    summary->mode.commands[summary->option].print_results(summary);
    
    fflush(stdin);
    scanf("%*c");
}

bool check_if_exit(Screen screens[], ActiveScreen *active)
{
    Screen *mainScreen = &screens[MAIN_MENU];
    return active->current == mainScreen && active->choice == 6;
}
