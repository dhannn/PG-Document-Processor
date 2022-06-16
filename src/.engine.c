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
 * get_analyzer_outfile_name()
 * checks if keyword is in the document and displays error if not
 * 
 * @param       ActiveScreen*           the pointer to the current screen
 * @param       Summary*                variables relevant for processing
 */
void __get_analyzer_outfile_name(ActiveScreen *active, Summary *summary);

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
        // TODO: @gwen extract method
        /*************************************************************************/
        int i;
        int numTokenList = 1;
        TokenList **tokenlists = malloc(sizeof(TokenList*));

        for(i = 0; summary->corpusData[i] != NULL; i++) {
            char *corpusData = summary->corpusData[i];

            if(i >= numTokenList) {
                numTokenList *= 4;
                tokenlists = realloc(tokenlists, sizeof(TokenList*) * numTokenList);
            }
                            
            tokenlists[i] = tokenize_string(corpusData, false);
        }
        tokenlists[i] = NULL;
        summary->corpusTokens = tokenlists;
        /*************************************************************************/
    }
}

void do_clean(ActiveScreen *active, Summary *summary, Config *config)
{
    __prepare_inputs(active, summary, config);
    restart_screen();
    __read_data(active, summary, config);

    go_to_screen(active, summary, CLEAN_DOCUMENT_MENU);
}

void do_s_analyze(ActiveScreen *active, Summary *summary, Config *config) 
{
    __prepare_inputs(active, summary, config);
    restart_screen();
    __read_data(active, summary, config);

    go_to_screen(active, summary, SINGLE_ANALYZE_DOCUMENT_MENU);
}

void do_chosen_mode(ActiveScreen *active, Summary *summary, Config *config) 
{
    ModeIndex index = summary->mode.index;
    ScreenTag resultingScreenIndex[] = {
        CLEAN_DOCUMENT_MENU,
        SINGLE_ANALYZE_DOCUMENT_MENU,
        MULTI_ANALYZE_DOCUMENT_MENU
    };

    __prepare_inputs(active, summary, config);

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

    while(in == 'Y' || in == 'y') {
        go_to_screen(active, summary, CLEAN_DOCUMENT_MENU);
        
        set_option(summary, *config, choice);
        execute_summary(summary);

        restart_screen();
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

    restart_screen();
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

    restart_screen();
    execute_summary(summary);

    __get_analyzer_outfile_name(active, summary);
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

void do_tfidf(ActiveScreen *active, Summary *summary, Config *config) 
{
    int choice = active->choice - 1;
    set_option(summary, *config, choice);

    __get_analyzer_outfile_name(active, summary);
    set_outfile(summary, *config, active->strInput);
    
    restart_screen();
    execute_summary(summary);

    save_results(active, summary, config);
    destroy_summary(summary);

    go_to_screen(active, summary, MAIN_MENU);
}

void do_doc_similarity(ActiveScreen *active, Summary *summary, Config *config) 
{
    
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
