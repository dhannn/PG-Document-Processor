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

/* -------------------------------------------------------------------------- */
/*                            DISPLAY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    char optionName[MAX_CHAR];
    void (*do_option)(Screen, Summary, Config);
} ScreenOption;

typedef struct _activeScreen ActiveScreen;

typedef struct { 
    char **header;
    char *prompt;
    ScreenOption options[MAX_OPTIONS];
    void (*get_input)(ActiveScreen*);
    int backIndex;
} Screen;

struct _activeScreen {
    Screen *screen;
    int nInput;
    char strInput[MAX_CHAR];
};

/* -------------------------------------------------------------------------- */
/*                            SUMMARY.C STRUCTURES                            */
/* -------------------------------------------------------------------------- */

typedef struct _summary Summary;
typedef struct _config Config;

typedef struct {
    const char *name;
    char *data;
} MetadataItem;

typedef struct {
    char *name;
    void (*execute_command)();
    void (*report_results)();
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

void initialize_screens(Screen screens[]);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(Screen screens[], ActiveScreen *active, int index);
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

const Mode MODES[3] = {
    {.index = CLEAN, .clean_or_analyze = clean_data}, 
    {.index = ANALYZE_SINGLE, .clean_or_analyze = analyze_data__single},
    {.index = ANALYZE_MULTI, .clean_or_analyze = analyze_data__multi}
};


#endif
