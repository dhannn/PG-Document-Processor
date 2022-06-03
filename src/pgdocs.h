#ifndef PG_DOCS_H
#define PG_DOCS_H
#include "deps/internals.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_CHAR        1024
#define MAX_OPTIONS     10
#define MAX_SCREENS     12
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

/* -------------------------------------------------------------------------- */
/*                            DISPLAY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef enum {
    CONFIG, 
    MAIN_MENU,
    ENTER_FILENAME_MENU,
    CLEAN_DOCUMENT_MENU,
    SINGLE_ANALYZE_DOCUMENT_MENU,
    MULTI_ANALYZE_DOCUMENT_MENU,
    ENTER_N_MENU,
    ENTER_KEYWORD_MENU,
    ENTER_NUMBER_CLUSTERS_MENU, 
    ENTER_PATH_MENU,
    ENTER_NUMBER_CHAR_MENU,
    ENTER_MULTISELECT_BOOL_MENU,
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
    void (*execute_command)(Summary*);
    void (*report_results)(Summary*);
} Command;

typedef enum {
    CLEAN,
    ANALYZE_SINGLE,
    ANALYZE_MULTI
} ModeIndex;

typedef struct {
    Command commands[MAX_OPTIONS];
    void (*clean_or_analyze)();
    ModeIndex index;
} Mode;

typedef struct {
    int i;                      // placeholder for int variable
    char *s;                    // placeholder for string variable
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
    char *rawDocumentPath;      // path of raw PG documents
    char *cleanedDocumentPath;  // path of cleaned documents
    char *analysisOutputPath;   // path of output files (i.e., word count,...)
    int numChar;                // number of character to be read from content
    bool multiSelect;           // allow to choose more than one option
};

/* -------------------------------------------------------------------------- */
/*                        DISPLAY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */

void initialize_screens(ActiveScreen *screens);
void display_screen(ActiveScreen *active);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(ActiveScreen *active, int index);
void get_int(ActiveScreen *active);
void get_str(ActiveScreen *active);
void print_metadata(char *metadataName[], char *metadata[], int maxMetadata);

/* -------------------------------------------------------------------------- */
/*                        SUMMARY.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// encapsulates the request and the needed variables in one structure
void set_mode(Summary *summary, ModeIndex mode);
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
void clean_up_reader();

/* -------------------------------------------------------------------------- */
/*                       METADATA.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
// sets and deletes the metadata
void initialize_metadata(MetadataItem metadataItems[]);
void set_metadata(MetadataItem items[], int metadataIndex, char *data);
int get_metadata_index(MetadataItem metadataItems[], char *str);
void delete_metadata(MetadataItem metadata[]);

/* -------------------------------------------------------------------------- */
/*                        CLEANER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */
void clean_data(Summary *summary, Config config);


/* -------------------------------------------------------------------------- */
/*                       ANALYZER.C FUNCTION PROTOTYPES                       */
/* -------------------------------------------------------------------------- */

void analyze_data__single(Summary *summary, Config config);
void analyze_data__multi(Summary *summary, Config config);

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


void load_help(ActiveScreen* active, Summary *summary, Config *config);
void return_screen(ActiveScreen* active, Summary *summary, Config *config);
void get_filename_for_processing(ActiveScreen* active, Summary *summary, Config *config);
void choose_option(ActiveScreen* active, Summary *summary, Config *config);
void do_processing(ActiveScreen* active, Summary *summary, Config *config);
bool check_if_exit(Screen screens[], ActiveScreen *active);

#endif
