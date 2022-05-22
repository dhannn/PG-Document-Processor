#ifndef PG_DOCS_H
#define PG_DOCS_H

#include <stdio.h>
#include <stdbool.h>

typedef struct _tokenList TokenList;
typedef struct _hashTable HashTable;

#define NUM_OF_METADATA 5

typedef struct {
    const char *name;
    char *data;
} MetadataItem;

typedef enum {
    CLEAN, 
    ANALYZE
} Mode;

#define MAX_METADATA_ITEMS 5

typedef struct {
    MetadataItem metadataItems[NUM_OF_METADATA];
} Metadata;

typedef struct {
    int i;                      // placeholder for int variable
    char *s;                    // placeholder for string variable
} AdditionalOptions;

typedef struct {
    Mode mode;                  // processing mode (i.e. clean or analyze)
    FILE *inFile;               // file pointer to input 
    FILE *outFile;              // file pointer to output
    unsigned int options;       // chosen option for cleaning or analysis
    Metadata metadata;          // info about the document
    char *inData;               // string of data to be processed
    TokenList *tokenList;       // tokenized version of the input
    char *outData;              // string of data to be reported
    AdditionalOptions addOpts;  // additional options
} Summary;

typedef struct {
    bool isInitialized;         // whether config is initialized or not
    char *rawDocumentPath;      // path of raw PG documents
    char *cleanedDocumentPath;  // path of cleaned documents
    char *analysisOutputPath;   // path of output files (i.e., word count,...)
    int numChar;                // number of character to be read from content
    bool multiSelect;           // allow to choose more than one option
} Config;

typedef struct {
    char *name;                                 // name of analysis option
    void (*do_analysis)(Summary*, Config);      // function pointer to analysis
    void (*report_analysis)(Summary*, Config);  // function pointer to report
    char *fileSuffix;                           // name of analysis option
} AnalyzerOption;

typedef struct {
    char *name;                                 // name of cleaning option
    void (*clean)(Summary*, Config);            // function pointer to cleaning
    void (*report_cleaned)(Summary*, Config);   // function pointer to report
} CleanerOption;

typedef struct _screen Screen;

typedef struct {
    int index;
    const char *optionName;
    Screen *next_screen;
    Screen *previous_screen;
} ScreenOption;

struct _screen{
    const char **header;
    int n;
    char *input;
    const ScreenOption *options;
};

#define MAX_ANALYZER_OPTIONS 3

TokenList *remove_duplicate_tokens(TokenList*);

Metadata initialize_metadata();
void set_metadata(MetadataItem items[], int metadataIndex, char *data);
int get_metadata_index(MetadataItem metadataItems[], char *str);
void delete_metadata(Metadata *metadata);

Screen *initialize_screens();

void read_file(Summary *summary, Config config);
void seek_metadata(FILE *file, MetadataItem items[], int buffSize);
void read_metadata(FILE *file, MetadataItem items[], Mode mode);
void read_content(FILE *file, char **inputData, int maxChar);
void clean_up_reader();

void set_mode(Summary *summary, Mode mode);
void set_infile(Summary *summary, char *filename);
void set_outfile(Summary *summary, char *filename);

void set_options(Summary *summary, Config config, int rawInput);
void set_additional_options(Summary *summary, AdditionalOptions addopts);
void execute_summary(Summary *summary, Config config);

void close_files(Summary *summary);
void destroy_summary(Summary* summary);

void analyze_data(Summary *summary, Config config);
TokenList *convert_to_ngrams(TokenList *tl, int n);
void get_word_count(Summary *summary, Config config);
void report_token_frequency(Summary *summary, Config config);
void get_ngram_count(Summary *summary, Config config);
void report_ngram_count(Summary *summary, Config config);
void get_concordance(Summary *summary, Config config);
void report_concordance(Summary *summary, Config config);

Metadata initialize_metadata();
void delete_metadata(Metadata *metadata);

extern const AnalyzerOption ANALYZER_OPTIONS[];

#endif
