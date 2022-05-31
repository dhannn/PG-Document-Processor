#ifndef PG_DOCS_H
#define PG_DOCS_H
#include <stdbool.h>

#define MAX_CHAR 1024

typedef struct _screen Screen;

typedef struct {
    char *optionName;
    Screen *resultingScreen;
    bool pointsToCommand;
} ScreenOption;

struct _screen { 
    char **header;
    char *prompt;
    ScreenOption *options;
    void (*get_input)(ActiveScreen);
    int backIndex;
};

typedef struct {
    Screen *screen;
    int nInput;
    char strInput[MAX_CHAR];
} ActiveScreen;

typedef struct {
    const char *name;
    char *data;
} MetadataItem;

typedef enum {
    CLEAN, 
    ANALYZE
} Mode;

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

void initialize_screens(Screen screens[]);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(Screen *screens[], ActiveScreen *active);
void get_int(ActiveScreen *active);
void get_str(ActiveScreen *active);
void print_metadata(char *metadataName[], char *metadata[], int maxMetadata);

void read_file(Summary *summary, Config config);
void seek_metadata(FILE *file, MetadataItem items[], int buffSize);
void read_metadata(FILE *file, MetadataItem items[], Mode mode);
void read_content(FILE *file, char **inputData, int maxChar);
void clean_up_reader();

#endif
