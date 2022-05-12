#include <stdio.h>
#include <stdbool.h>

typedef struct _tokenList TokenList;
typedef struct _hashTable HashTable;

#define NUM_OF_METADATA 5

typedef struct {
    char *name;
    char *data;
} MetadataItem;

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
    FILE *inFile;
    FILE *outFile;
    Metadata metadata;
    unsigned int options;
    char *inData;
    TokenList *tokenList;
    char *outData;
} Summary;

typedef struct {
    bool isInitialized;
    char *inputPath;
    char *outputPath;
    char *corpusPath;
    int numChar;
    bool multiSelect;
} Config;

typedef struct {
    char *name;
    void (*do_analysis)(Summary*, Config);
} AnalyzerOption;

#define MAX_ANALYZER_OPTIONS 3

void read_clean_file(Summary *summary, Config config, char *filename);
// TokenList *tokenize(char *input, bool includeSpace);
TokenList *convert_to_ngrams(TokenList *tl, int n);
void delete_tokens(TokenList *tokenList);
void get_word_count(Summary *summary, Config config);
void get_ngram_count(Summary *summary, Config config);
void get_concordance(Summary *summary, Config config);
