#ifndef PG_DOCS_H
#define PG_DOCS_H
#include "deps/internals.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_CHAR        1024
#define MAX_OPTIONS     10
#define MAX_SCREENS     15
#define MAX_METADATA    5

/**
 * Basic Program Structure
 * 
 * Engine               Combines all the modules
 * |
 * + Config             Sets config structure
 * |
 * + Display            Acts as the "view" and "controller" of the software
 * |
 * + Summary            
 *   |
 *   + Analyzer
 *   |
 *   + Cleaner
 *   |
 *   + Reader
 *     |
 *     + Metadata
 */



typedef struct _summary Summary;
typedef struct _config Config;

typedef struct _activeScreen ActiveScreen;

typedef enum {
    ERR_FILE_NOT_FOUND,   
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
    void (*get_input)(ActiveScreen*);
    int backIndex;
} Screen;

struct _activeScreen {
    Screen *current;
    Screen screens[MAX_SCREENS];
    int nInput;
    char strInput[MAX_CHAR];
};

/* -------------------------------------------------------------------------- */
/*                            SUMMARY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    const char *name;
    char *data;
} MetadataItem;

typedef struct {
    char *name;
    void (*execute_command)(Summary*, Config);
    void (*report_results)(Summary*, Config);
} Command;

typedef enum {
    CLEAN,
    ANALYZE_SINGLE,
    ANALYZE_MULTI
} ModeIndex;

typedef struct {
    Command commands[MAX_OPTIONS];
    void (*clean_or_analyze)(Summary*, Config);
    ModeIndex index;
} Mode;

#define MAX_ADD_OPTS 4

typedef struct {
    int i[MAX_ADD_OPTS];                // placeholder for int variable
    char s[MAX_ADD_OPTS][MAX_CHAR];     // placeholder for string variable
} AdditionalOptions;

struct _summary {
    Mode mode;                  // processing mode (i.e. clean or analyze)
    FILE *inFile;               // file pointer to input 
    FILE *outFile;              // file pointer to output
    unsigned int options;       // chosen option for cleaning or analysis
    MetadataItem metadata[MAX_METADATA];    // info about the document
    char *inData;               // string of data to be processed
    TokenList *tokenList;       // tokenized version of the input
    char *outData;              // string of data to be reported
    AdditionalOptions addOpts;  // additional options
};

struct _config {
    bool isInitialized;         // whether config is initialized or not
    char rawDocumentPath[MAX_CHAR];      // path of raw PG documents
    char cleanedDocumentPath[MAX_CHAR];  // path of cleaned documents
    char analysisOutputPath[MAX_CHAR];   // path of output files (e.g. word count)
    int numChar;                // number of character to be read from content
    bool multiSelect;           // allow to choose more than one option
};

/* -------------------------------------------------------------------------- */
/*                        DISPLAY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */

void initialize_screens(ActiveScreen *screens);
void display_screen(ActiveScreen *active, Summary *summary);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(ActiveScreen *active, int index);
void get_int(ActiveScreen *active);
void get_str(ActiveScreen *active);
void print_metadata(char metadataName[][MAX_CHAR], char metadata[][MAX_CHAR], int maxMetadata);
void print_token_frequency(FILE *outfile, char *results);
void print_cleaned(FILE *outfile, MetadataItem metadata[], char *results);

/* -------------------------------------------------------------------------- */
/*                        SUMMARY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// encapsulates the request and the needed variables in one structure
void set_mode(Summary *summary, ModeIndex mode);
void set_infile(Summary *summary, Config config, char *filename);
void set_outfile(Summary *summary, Config config, char *filename);
void set_options(Summary *summary, Config config, int rawInput);
void execute_summary(Summary *summary, Config config);
void destroy_summary(Summary *summary);


/* -------------------------------------------------------------------------- */
/*                        READER.C FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
// reads the metadata and content of the file
void read_file(Summary *summary, Config config);
void seek_metadata(FILE *file, MetadataItem items[], int buffSize);
void read_metadata(FILE *file, MetadataItem items[], ModeIndex mode);
void read_content(FILE *file, char **inputData, int maxChar);
void clean_up_reader(Summary *summary);

/* -------------------------------------------------------------------------- */
/*                       METADATA.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// sets and deletes the metadata
void initialize_metadata(MetadataItem metadataItems[]);
void set_metadata(MetadataItem items[], int metadataIndex, char *data);
int get_metadata_index(MetadataItem metadataItems[], char *str);
void delete_metadata(MetadataItem metadata[]);

/* -------------------------------------------------------------------------- */
/*                        CONFIG.C FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
bool check_config_initialized();
void load_config(Config *config);
void set_config_path(Config *config, int index, char *path);
void set_config_int(Config *config, int index, int data);

/* -------------------------------------------------------------------------- */
/*                        CLEANER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
void clean_data(Summary *summary, Config config);
void clean_all(Summary *summary, Config config);

/* -------------------------------------------------------------------------- */
/*                       ANALYZER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */

void analyze_data__single(Summary *summary, Config config);
void analyze_data__multi(Summary *summary, Config config);
void report_token_frequency(Summary *summary, Config config);

#define MAX_ANALYZER_OPTIONS 3
#define MAX_CLEANER_OPTIONS 5

void read_clean_file(Summary *summary, Config config, char *filename);
TokenList *tokenize(char *input, bool includeSpace);

void to_lowercase(Summary *summary, Config config);
void remove_special(Summary *summary, Config config);
void remove_numbers(Summary *summary, Config config);
void clean_whitespace(Summary *summary, Config config);
void remove_stopword(Summary *summary, Config config);
void report_cleaned(Summary *summary, Config config);

TokenList *convert_to_ngrams(TokenList *tl, int n);
void delete_tokens(TokenList *tokenList);
void get_word_count(Summary *summary, Config config);
void get_ngram_count(Summary *summary, Config config);
void get_concordance(Summary *summary, Config config);

void report_word_count(Summary *summary, Config config);
void report_ngram_count(Summary *summary, Config config);
void report_concordance(Summary *summary, Config config);

void get_tfidf(Summary *summary, Config config);
void get_document_similarity(Summary *summary, Config config);

void report_tfidf(Summary *summary, Config config);
void report_document_similarity(Summary *summary, Config config);

/* -------------------------------------------------------------------------- */
/*                        ENGINE.C FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */

void initialize_config(ActiveScreen *active, Config *config);
void reset_config(ActiveScreen *active, Summary *summary, Config *config);
void load_help(ActiveScreen* active, Summary *summary, Config *config);
void return_screen(ActiveScreen* active, Summary *summary, Config *config);
void get_filename_for_processing(ActiveScreen* active, Summary *summary, Config *config);
void choose_option(ActiveScreen* active, Summary *summary, Config *config);
void get_add_opts(ActiveScreen* active, Summary *summary, Config *config);
void do_processing(ActiveScreen* active, Summary *summary, Config *config);
void save_results(ActiveScreen* active, Summary *summary, Config *config);
bool check_if_exit(Screen screens[], ActiveScreen *active);

#endif
