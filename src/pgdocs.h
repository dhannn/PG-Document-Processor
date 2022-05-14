#include <stdio.h>
#include <stdbool.h>

typedef struct _tokenList TokenList;
typedef struct _hashTable HashTable;

#define NUM_OF_METADATA 5

typedef struct {
    char *name;
    char *data;
} MetadataItem;

typedef enum {
    CLEAN, 
    ANALYZE
} Mode;

// const char **METADATA_ITEM_NAMES = {
//     "Title",
//     "Author", 
//     "Release Date",
//     "Language",
//     "Produced by"
// };

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
    Metadata metadata;          // info about the document
    unsigned int options;       // chosen option for cleaning or analysis
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
} AnalyzerOption;

typedef struct {
    char *name;                                 // name of cleaning option
    void (*clean)(Summary*, Config);            // function pointer to cleaning
    void (*report_cleaned)(Summary*, Config);   // function pointer to report
} CleanerOption;

#define MAX_ANALYZER_OPTIONS 3

void read_clean_file(Summary *summary, Config config, char *filename);
// TokenList *tokenize(char *input, bool includeSpace);
TokenList *convert_to_ngrams(TokenList *tl, int n);
void delete_tokens(TokenList *tokenList);
void get_word_count(Summary *summary, Config config);
void report_word_count(Summary *summary, Config config);
void get_ngram_count(Summary *summary, Config config);
void report_ngram_count(Summary *summary, Config config);
void get_concordance(Summary *summary, Config config);
void report_concordance(Summary *summary, Config config);
