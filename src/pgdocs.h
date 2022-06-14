/*******************************************************************************
 * 
 * FILE             pgdocs.h
 * LAST MODIFIED    06-11-2022
 * 
 * DESCRIPTION
 *      This file contains the function, structure and constants 
 *      declarations used throughout the program.
 * 
 ******************************************************************************/

#ifndef PG_DOCS_H
#define PG_DOCS_H

#include "deps/internals.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#define MAX_CHAR        1024
#define MAX_OPTIONS     10
#define MAX_SCREENS     15
#define MAX_METADATA    5

#define MAX_ANALYZER_OPTIONS 3
#define MAX_CLEANER_OPTIONS 5

#define MAX_ADD_OPTS 4

typedef struct _summary Summary;
typedef struct _config Config;

typedef struct _activeScreen ActiveScreen;

typedef enum {
    ERR_FILE_NOT_FOUND,
    ERR_INVALID_CHOICE,   
    ERR_INVALID_INT,
    ERR_INVALID_KEYWORD,
    ERR_MALLOC_FAILED   
} ErrorCode;


/* -------------------------------------------------------------------------- */
/*                            DISPLAY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef enum {
    CONFIG, 
    MAIN_MENU,
    ENTER_INPUT_FILE_MENU,
    CLEAN_DOCUMENT_MENU,
    SINGLE_ANALYZE_DOCUMENT_MENU,
    MULTI_ANALYZE_DOCUMENT_MENU,
    ENTER_N_MENU,
    ENTER_KEYWORD_MENU,
    ENTER_NUMBER_CLUSTERS_MENU, 
    ENTER_RAW_PATH_MENU,
    ENTER_CLEANED_PATH_MENU,
    ENTER_ANALYSIS_PATH_MENU,
    ENTER_NUMBER_CHAR_MENU,
    ENTER_MULTISELECT_BOOL_MENU,
    ENTER_OUTPUT_FILE_MENU,
    EXIT
} ScreenTag;

typedef struct {
    char optionName[MAX_CHAR];
    void (*do_option)(ActiveScreen*, Summary*, Config*);
} ScreenOption;

typedef struct { 
    char **header;
    char *prompt;
    ScreenOption options[MAX_OPTIONS];
    int index;
    int numOptions;
    void (*get_input)(ActiveScreen*);
    int backIndex;
} Screen;

struct _activeScreen {
    Screen *current;
    Screen screens[MAX_SCREENS];
    int choice;
    int nInput;
    char strInput[MAX_CHAR];
};

/* -------------------------------------------------------------------------- */
/*                            SUMMARY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    const char *fieldName;
    char *data;
} MetadataItem;

typedef struct {
    char *name;
    void (*execute_command)(Summary*);
    void (*report_results)(Summary*);
    void (*print_results)(Summary*);
    char *fileSuffix;
    int usedAddInt;
    int usedAddStr;
} Command;

typedef enum {
    CLEAN,
    ANALYZE_SINGLE,
    ANALYZE_MULTI
} ModeIndex;

typedef struct {
    Command commands[MAX_OPTIONS];
    void (*clean_or_analyze)(Summary*);
    ModeIndex index;
} Mode;

typedef struct {
    int i[MAX_ADD_OPTS];                // placeholder for int variable
    char s[MAX_ADD_OPTS][MAX_CHAR];     // placeholder for string variable
} AdditionalOptions;

struct _summary {
    Mode mode;                  // processing mode (i.e. clean or analyze)
    char infilename[MAX_CHAR];
    FILE *infile;               // file pointer to input 
    FILE **corpus;              // array of file pointers to the files
    FILE *outfile;              // file pointer to output
    unsigned int option;        // chosen option for cleaning or analysis
    MetadataItem metadata[MAX_METADATA];    // info about the document
    char *inData;               // string of data to be processed
    char **corpusData;          // string of data of the corpus
    TokenList *tokenList;       // tokenized version of the input
    TokenList **corpusTokens;   // tokenized version of the corpus
    char *outData;              // string of data to be reported
    AdditionalOptions addOpts;  // additional options
};

struct _config {
    bool isInitialized;         // whether config is initialized or not
    char rawDocumentPath[MAX_CHAR];      // path of raw PG documents
    char cleanedDocumentPath[MAX_CHAR];  // path of cleaned documents
    char analysisOutputPath[MAX_CHAR];   // path of output files
    int numChar;                // number of character to be read from content
    int numDocs;                // number of documents to be read from corpus
};

/* -------------------------------------------------------------------------- */
/*                        DISPLAY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */

/* ---------------------------- Screen Functions ---------------------------- */
/**
 * initialize_screens()
 * initializes the screens to be displayed
 * 
 * @param       ActiveScreen*   pointer to the current screen
 */
void initialize_screens(ActiveScreen *screen);

/**
 * go_to_screen()
 * sets the active screen to the specified screen
 * 
 * @param       ActiveScreen*   pointer of the current screen
 * @param       int             the index of the screen to be set
 */
void go_to_screen(ActiveScreen *active, Summary *summary, int index);

/**
 * display_screen()
 * displays the current screen and its constituents
 * 
 * @param       ActiveScreen*   pointer to the current screen to be displayed
 * @param       Summary*        pointer of the summary containing metadata
 */
void display_screen(ActiveScreen *active, Summary *summary);

/**
 * display_error()
 * informs the user of an error based on a code
 * 
 * @param       ErrorCode       the code pertaining to the error type
 */
void display_error(ErrorCode errorCode);

/* ----------------------------- Input Functions ---------------------------- */
/**
 * get_choice()
 * asks the user to pick a choice from the screen options and sets it to
 * the choice field of the ActiveScreen structure
 * 
 * @note        this function is considered as a function implementation of 
 *              the interface, void (*get_input)(ActiveScreen*), found in
 *              the Screen structure
 * 
 * @param       ActiveScreen*   pointer to the current screen
 */
void get_choice(ActiveScreen *active);

/**
 * get_int()
 * asks the user to enter an integer output and sets it to the nInput field
 * of the ActiveScreen structure
 * 
 * @note        this function is considered as a function implementation of 
 *              the interface, void (*get_input)(ActiveScreen*), found in
 *              the Screen structure
 * 
 * @param       ActiveScreen*   pointer to the current screen
 */
void get_int(ActiveScreen *active);

/**
 * get_int()
 * asks the user to enter an integer output and sets it to the nInput
 * field of the ActiveScreen structure
 * 
 * @note        this function is considered as a function implementation of 
 *              the interface, void (*get_input)(ActiveScreen*), found in
 *              the Screen structure
 * 
 * @param       ActiveScreen*   pointer to the current screen
 */
void get_str(ActiveScreen *active);

/* ---------------------------- Update Functions ---------------------------- */
/**
 * update_reading()
 * updates the user about the progress of file reading by providing feedback
 * 
 * @param       int             the number of characters read
 * @param       int             the number of words read
 */
void update_reading(int characters, int words);

/**
 * update_tokenizing()
 * updates the user about the progress of tokenization by providing feedback
 * 
 * @param       char*           current string tokenized
 * @param       int             the number of tokens made
 */
void update_tokenizing(char *current, int tokens);

/**
 * update_processing()
 * updates the user about the progress of doc processing by providing feedback
 * 
 * @param       int             the current number of tokens processed
 * @param       int             the maximum number of tokens
 */
void update_processing(int progress, int max);

/**
 * update_reporting()
 * updates the user about the progress of reporting by providing feedback
 * 
 * @param       int             the current number of tokens reported
 * @param       int             the maximum number of tokens
 */
void update_reporting(int progress, int max);

/* ---------------------- Printing of Summary Functions --------------------- */
/**
 * print_metadata()
 * prints the read metadata information
 * 
 * @param       char[][]        the list of field names for the metadata 
 * @param       char[][]        the list of the actual data 
 */
void print_metadata(char fieldName[][MAX_CHAR], char data[][MAX_CHAR]);

/**
 * print_cleaned()
 * prints the cleaned outdata string to the file
 * 
 * @note        this function is an implementation of the interface, 
 *              void (*print_results)(Summary*) found in the Command structure
 * 
 * 
 * @param       Summary*        summary containing the output data
 */
void print_cleaned(Summary *summary);

/**
 * print_token_frequency()
 * prints the outdata string showing token frequency string to the file
 * 
 * @note        this function is an implementation of the interface, 
 *              void (*print_results)(Summary*) found in the Command structure
 * 
 * 
 * @param       Summary*        summary containing the output data
 */
void print_token_frequency(Summary *summary);

/**
 * print_concordance()
 * prints the outdata string showing concordance to the file
 * 
 * @note        this function is an implementation of the interface, 
 *              void (*print_results)(Summary*) found in the Command structure
 * 
 * 
 * @param       Summary*        summary containing the output data
 */
void print_concordance(Summary *summary);

/* -------------------------------------------------------------------------- */
/*                        SUMMARY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// encapsulates the request and the needed variables in one structure

/**
 * execute_summary()
 * runs the command based on the entered information
 * 
 * @param       Summary*        contains the necessary inputs for processing
 */
void execute_summary(Summary *summary);

/**
 * destroy_summary()
 * deallocates all the dynamically-allocated fields in the summary structure
 * 
 * @param       Summary*        contains the necessary inputs for processing
 */
void destroy_summary(Summary *summary);
/**
 * set_mode()
 * sets the mode of processing (either cleaning or analyzing)
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       ModeIndex       the index for the chosen mode
 */
void set_mode(Summary *summary, ModeIndex mode);

/**
 * set_option()
 * sets the command chosen
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       ModeIndex       the index for the chosen mode
 */
void set_option(Summary *summary, Config config, int rawInput);

/**
 * set_infile()
 * sets the input file and opens it
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       Config          contains the paths for different modes
 * @param       char*           filename of the input file
 */
void set_infile(Summary *summary, Config config, char *filename);

/**
 * set_outfile()
 * sets the output file and opens it
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       Config          contains the paths for different modes
 * @param       char*           filename of the output file
 */
void set_outfile(Summary *summary, Config config, char *filename);

/**
 * set_add_str()
 * adds a string to the additional options 
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       char*           string data to be added
 */
void set_add_str(Summary *summary, char *addStr);

/**
 * set_add_int()
 * adds an integer to the additional options 
 * 
 * @param       Summary*        contains the necessary inputs for processing
 * @param       int             integer data to be added
 */
void set_add_int(Summary *summary, int addInt);


/* -------------------------------------------------------------------------- */
/*                        READER.C FUNCTION PROTOTYPES                        */
/* ---------------------------------------------------  ----------------------- */
/**
 * read_file()
 * reads the metadata and the content of a file
 * 
 * @pre         infile element of the Summary struct is valid
 * @param       Summary*            structure containing the necessary inputs
 * @param       Config              structure containing the configurations
 */
void read_file(Summary *summary, Config config);

void initialize_corpus(Summary *summary, Config config);
void read_corpus(Summary *summary, Config config);

/**
 * seek_metadata()
 * sets the FILE pointer to the start of the metadata 
 * 
 * @pre         infile is valid and not NULL
 * @param       FILE*               input file to be read
 * @param       MetadataItem[]      the array of metadata items to be read
 */
void seek_metadata(FILE *file, MetadataItem items[]);

/**
 * read_metadata()
 * reads the metadata and stores it to the metadata array
 * 
 * @pre         infile is valid and not NULL
 * @param       FILE*               input file to be read
 * @param       MetadataItem[]      the array of metadata items to be read
 */
void read_metadata(FILE *file, MetadataItem items[], ModeIndex mode);

/**
 * read_content()
 * reads the content of the input file
 * 
 * @pre         infile is valid and not NULL
 * @param       FILE*               input file to be read
 * @param       char**              the pointer to the input string read
 * @param       int                 maximum number of char to be read; 0 if all
 */
void read_content(FILE *file, char **inputData, int maxChar);

/**
 * read_metadata()
 * reads the metadata and stores it to the metadata array
 * 
 * @pre         infile is valid and not NULL
 * @param       FILE*               input file to be read
 * @param       MetadataItem[]      the array of metadata items to be read
 */
void clean_up_reader(Summary *summary);

/* -------------------------------------------------------------------------- */
/*                       METADATA.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// sets and deletes the metadata

/**
 * initialize_metadata()
 * initializes the metadata field name and data
 * 
 * @param       MetadataItem[]      the array of metadata items to be set
 */
void initialize_metadata(MetadataItem metadataItems[]);

/**
 * set_metadata()
 * sets the specified metadata field with data
 * 
 * @param       MetadataItem[]      the array of metadata items to be set
 * @param       int                 the index to the field to be set
 * @param       char*               the data associated with the field
 */
void set_metadata(MetadataItem items[], int metadataIndex, char *data);

/**
 * get_metadata_index()
 * returns the number of the index associated with the field name 
 * 
 * @param       MetadataItem[]      the array of metadata items to be searched
 * @param       char*               the name of the field to be searched
 * @return      the index of the field name if found; -1 if otherwise
 */
int get_metadata_index(MetadataItem metadataItems[], char *fieldName);

/**
 * get_metadata_index()
 * returns the number of the index associated with the field name 
 * 
 * @param       MetadataItem[]      the array of metadata items to be searched
 * @param       char*               the name of the field to be searched
 * @return      the index of the field name if found; -1 if otherwise
 */
void delete_metadata(MetadataItem metadata[]);

/* -------------------------------------------------------------------------- */
/*                        CONFIG.C FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/**
 * check_config_initialized()
 * check if the config file is initialized (i.e. config file exists)
 * 
 * @return      true if initialized; false if otherwise
 */
bool check_config_initialized();

/**
 * load_config()
 * initialize the config structure based on the file
 * 
 * @param       Config*             the structure containing all config fields
 */
void load_config(Config *config);

/**
 * set_config_path()
 * sets the path to configuration settings such as raw file path
 * 
 * @pre         index is a valid path config; path string should exist
 * @param       Config*             the structure containing all config fields
 * @param       int                 the index of the config field
 * @param       char*               path string of the chosen field 
 */
void set_config_path(Config *config, int index, char *path);\

/**
 * set_config_int()
 * sets the int to configuration settings such as number of char to be read
 * 
 * @pre         index is a valid int config; int data should be valid
 * @param       Config*             the structure containing all config fields
 * @param       int                 the index of the config field
 * @param       int                 the int data of the chosen field 
 */
void set_config_int(Config *config, int index, int data);

/* -------------------------------------------------------------------------- */
/*                        CLEANER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
/**
 * clean_data()
 * executes command and report results to the outdata of Summary
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void clean_data(Summary *summary);

/**
 * to_lowercase()
 * converts all the tokenized string to lowercase
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void to_lowercase(Summary *summary);

/**
 * remove_special_char()
 * removes the tokens tagged as special characters
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void remove_special_char(Summary *summary);

/**
 * remove_numbers()
 * removes the tokens tagged as numeric
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void remove_numbers(Summary *summary);

/**
 * clean_whitespace()
 * removes the tokens tagged as whitespace
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void clean_whitespace(Summary *summary);

/**
 * remove_stopwords()
 * removes the tokens considered as stopwords
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void remove_stopwords(Summary *summary);

/**
 * clean_all()
 * loops through all the cleaning commands
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void clean_all(Summary *summary);


/**
 * report_cleaned()
 * writes the newly-cleaned tokens to the outdata of the summary structure
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void report_cleaned(Summary *summary);

/* -------------------------------------------------------------------------- */
/*                       ANALYZER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
/**
 * analyze_data__single()
 * executes command and report results to the outdata of Summary
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void analyze_data__single(Summary *summary);

/**
 * analyze_data__multi()
 * executes command and report results to the outdata of Summary
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void analyze_data__multi(Summary *summary);

/**
 * get_word_count()
 * gets the word count of each token
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void get_word_count(Summary *summary);

/**
 * get_ngram_count()
 * gets the count of each ngram
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void get_ngram_count(Summary *summary);

/**
 * get_word_count()
 * gets the context of a keyword by searching through its occurences
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void get_concordance(Summary *summary);

/**
 * get_tfidf()
 * gets the tf-idf of each word
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void get_tfidf(Summary *summary);

/**
 * get_document_similarity()
 * calculates the similarity between two documents
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void get_document_similarity(Summary *summary);


/**
 * report_token_frequency()
 * writes the frequency of each token to the outdata
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void report_token_frequency(Summary *summary);

/**
 * report_concordance()
 * writes the occurences of the chosen keyword to the outdata
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void report_concordance(Summary *summary);

/**
 * report_concordance()
 * writes the tf-odf of each word to the outdata
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void report_tfidf(Summary *summary);

/**
 * report_concordance()
 * writes the similarity of two documents to outdata
 * 
 * @param       Summary*            contains the necessary input for processing
 */
void report_document_similarity(Summary *summary);

/* -------------------------------------------------------------------------- */
/*                        ENGINE.C FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */

void do_clean(ActiveScreen *active, Summary *summary, Config *config);
void do_s_analyze(ActiveScreen *active, Summary *summary, Config *config);
void do_m_analyze(ActiveScreen *active, Summary *summary, Config *config);
void do_clean_options(ActiveScreen *active, Summary *summary, Config *config);
void do_remove_special(ActiveScreen *active, Summary *summary, Config *config);
void do_remove_numbers(ActiveScreen *active, Summary *summary, Config *config);
void do_clean_whitespace(ActiveScreen *active, Summary *summary, Config *config);
void do_remove_stopwords(ActiveScreen *active, Summary *summary, Config *config);
void do_clean_all(ActiveScreen *active, Summary *summary, Config *config);
void do_word_count(ActiveScreen *active, Summary *summary, Config *config);
void do_ngram_count(ActiveScreen *active, Summary *summary, Config *config);
void do_concordance(ActiveScreen *active, Summary *summary, Config *config);
void do_tfidf(ActiveScreen *active, Summary *summary, Config *config);
void do_doc_similarity(ActiveScreen *active, Summary *summary, Config *config);

void initialize_config(ActiveScreen *active, Config *config);
void choose_option(ActiveScreen* active, Summary *summary, Config *config);
void reset_config(ActiveScreen *active, Summary *summary, Config *config);
void load_help(ActiveScreen* active, Summary *summary, Config *config);
void return_screen(ActiveScreen* active, Summary *summary, Config *config);
void get_filename(ActiveScreen* active, Summary *summary, Config *config);
void get_add_opts(ActiveScreen* active, Summary *summary, Config *config);
void do_processing(ActiveScreen* active, Summary *summary, Config *config);
void save_results(ActiveScreen* active, Summary *summary, Config *config);
bool check_if_exit(Screen screens[], ActiveScreen *active);

#endif
