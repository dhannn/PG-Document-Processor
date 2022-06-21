/*******************************************************************************
 * 
 * FILE             engine.c
 * LAST MODIFIED    06-17-2022
 * 
 * DESCRIPTION
 *      This file contains function implementations that combine
 *      the various lower-level modules. 
 * 
 ******************************************************************************/

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

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/**
 * prepare_inputs()
 * sets the summary variables necessary for processing
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 * @param       Config                  structure containing configurations
 */
void __prepare_inputs(ActiveScreen *active, Summary *summary, Config *config);
/**
 * read_data()
 * 
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 * @param       Config                  structure containing configurations
 */
void __read_data(ActiveScreen *active, Summary *summary, Config *config);
/**
 * get_analyzer_outfile_name()
 * checks if keyword is in the document and displays error if not
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 */
void __get_analyzer_outfile_name(ActiveScreen *active, Summary *summary);
/**
 * validate_file()
 * checks if file exists and displays error if invalid
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 * @param       Config                  structure containing configurations
 */
void __validate_file(ActiveScreen *active, Summary *summary, Config config);
/**
 * validate_n()
 * checks if n is valid and displays error if not
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 * @param       Config                  structure containing configurations
 */
void __validate_n(ActiveScreen *active, Summary *summary);
/**
 * validate_keywords()
 * checks if keyword is in the document and displays error if not
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 * @param       Config                  structure containing configurations
 */
void __validate_keyword(ActiveScreen *active, Summary *summary);
/**
 * __get_continue()
 * continuously accepts input until in is valid for continuing screen
 * 
 * @param       in                      user input to continue or not
 * @return      char value of valid input
 */
int __get_continue(char in);



/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

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
        ENTER_NUM_DOCS_BOOL_MENU
    };
    
    FILE *configFile = fopen("dat/config", "w");

    for(int i = 0; i < 5; i++) {
        int index = CONFIG_SCREENS[i];
        
        go_to_screen(active, NULL, index);

        if(i < 3) {
            set_config_path(config, i, active->strInput);
        } else {
            set_config_int(config, i, active->choice);
        }
    }
    
    fclose(configFile);
}

void do_chosen_mode(ActiveScreen *active, Summary *summary, Config *config) 
{
    ScreenTag resultingScreenIndex[] = {
        CLEAN_DOCUMENT_MENU,
        SINGLE_ANALYZE_DOCUMENT_MENU,
        MULTI_ANALYZE_DOCUMENT_MENU
    };

    __prepare_inputs(active, summary, config);
    ModeIndex index = summary->mode.index;

    if(index == ANALYZE_MULTI)
        initialize_corpus(summary, *config);

    restart_screen();
    __read_data(active, summary, config);

    go_to_screen(active, summary, resultingScreenIndex[index]);
}

void do_default_option(ActiveScreen *active, Summary *summary, Config *config)
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    restart_screen();
    execute_summary(summary);

    if(summary->mode.index == CLEAN)
        go_to_screen(active, summary, ENTER_OUTPUT_FILE_MENU);
    else
        __get_analyzer_outfile_name(active, summary);
    set_outfile(summary, *config, active->strInput);
    
    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_clean_options(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    CLEAR();
    execute_summary(summary);
    
    char in; 
    restart_screen();
    printf("Continue cleaning? [Y or N]: ");
    scanf(" %c", &in);

    if(in != 'Y' && in != 'y' && in != 'N' && in != 'N' && in != 'n') 
        in = __get_continue(in);

    while(in == 'Y' || in == 'y') {
        go_to_screen(active, summary, CLEAN_DOCUMENT_MENU);
        choice = active->choice - 1;
        
        if(active->choice == active->current->numOptions) {
            destroy_summary(summary);
            return;
        } 

        // while the option is viewing help
        while(active->choice == 7) {
            set_option(summary, *config, choice);
            load_help(active, summary, config);
            choice = active->choice - 1;

            if(active->choice == active->current->numOptions) {
                destroy_summary(summary);
                return;
            } 
        }

        set_option(summary, *config, choice);
        restart_screen();  

        execute_summary(summary);

        restart_screen();
        printf("Continue cleaning? [Y or N]: ");
        scanf(" %c", &in);

        if(in != 'Y' && in != 'y' && in != 'N' && in != 'N' && in != 'n') 
            in = __get_continue(in);
    }

    go_to_screen(active, summary, ENTER_OUTPUT_FILE_MENU);
    set_outfile(summary, *config, active->strInput);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_ngram_count(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    go_to_screen(active, summary, ENTER_N_MENU);

    __validate_n(active, summary);
    set_add_int(summary, active->nInput);

    restart_screen();
    execute_summary(summary);

    __get_analyzer_outfile_name(active, summary);
    set_outfile(summary, *config, active->strInput);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_concordance(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    go_to_screen(active, summary, ENTER_KEYWORD_MENU);
    __validate_keyword (active, summary);
    set_add_str(summary, active->strInput);

    go_to_screen(active, summary, ENTER_N_MENU);
    __validate_n(active, summary);
    set_add_int(summary, active->nInput); 

    restart_screen();
    execute_summary(summary);

    __get_analyzer_outfile_name(active, summary);
    set_outfile(summary, *config, active->strInput);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_doc_similarity(ActiveScreen *active, Summary *summary, Config *config) 
{
    return;   
}

void reset_config(ActiveScreen *active, Summary *summary, Config *config)
{
    remove(CONFIG_FILE);
}

#define HELP_FILE   "dat/help.txt"
#define MAX_HEIGHT  10

void load_help(ActiveScreen* active, Summary *summary, Config *config)
{
    FILE *help = fopen(HELP_FILE, "r");
    char buff[MAX_CHAR] = "";
    
    char *flag = fgets(buff, 85 + 1, help);
    
    int linesRead = 0;

    CLEAR();
    MOVE(1, 1);
    while(flag != NULL) {
        fprintf(stdout, "%s", buff);

        flag = fgets(buff, 85 + 1, help);
        linesRead++;

        if(linesRead % MAX_HEIGHT == 0) {
            printf("\n(Press any key to proceed)");
            fflush(stdin);
            scanf("%*c");
            CLEAR();
            MOVE(1, 1);
        }
    }

    fclose(help);

    go_to_screen(active, summary, active->current->index);
}

void return_screen(ActiveScreen* active, Summary *summary, Config *config)
{
    int index = active->current->backIndex;
    
    if(index == EXIT) {
        restart_screen();
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

/* -------------------------------------------------------------------------- */
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/* -------------------------------------------------------------------------- */

void __prepare_inputs(ActiveScreen *active, Summary *summary, Config *config)
{
    int mode = active->choice - 1;
    set_mode(summary, mode);

    go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);    
    set_infile(summary, *config, active->strInput);
    __validate_file(active, summary, *config);
}

void __read_data(ActiveScreen *active, Summary *summary, Config *config)
{
    ModeIndex index = summary->mode.index;

    read_file(summary, *config);
    summary->tokenList = tokenize_string(summary->inData, index == CLEAN);

    if(index == ANALYZE_MULTI) {
        read_corpus(summary, *config);
        summary->corpusTokens = tokenize_corpus(summary->corpusData);
    }
}

void __get_analyzer_outfile_name(ActiveScreen *active, Summary *summary)
{
    char filename[MAX_CHAR] = "";
    int option = summary->option;
    char *suffix = summary->mode.commands[option].fileSuffix;
    char *original = strtok(summary->infilename, ".");
    
    strcpy(filename, original);
    strcat(filename, "_");
    strcat(filename, suffix);
    strcat(filename, ".txt");

    strcpy(active->strInput, filename);
}

void __validate_file(ActiveScreen *active, Summary *summary, Config config)
{
    while(summary->infile == NULL) {
        display_error(ERR_FILE_NOT_FOUND);

        go_to_screen(active, summary, ENTER_INPUT_FILE_MENU);
        set_infile(summary, config, active->strInput);
    }
}

void __validate_n (ActiveScreen *active, Summary *summary)
{
    while(active->nInput > 4 || active->nInput < 1) {
        display_error(ERR_INVALID_INT);
        go_to_screen(active, summary, ENTER_N_MENU);
    }
}

void __validate_keyword(ActiveScreen *active, Summary *summary)
{
    while(!is_token_found(summary->tokenList, active->strInput)) {
        display_error(ERR_INVALID_KEYWORD);
        go_to_screen(active, summary, ENTER_KEYWORD_MENU);
    }
}

int __get_continue(char in)
{
    while(in != 'Y' && in != 'y' && in != 'N' && in != 'N' && in != 'n') {
        CLEAR();
        MOVE(1,1);
        printf("Invalid input!\nContinue cleaning? [Y or N]: ");
        scanf(" %c", &in);
    }
}
