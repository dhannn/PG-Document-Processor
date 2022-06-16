#include "pgdocs.h"
#include "deps/internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

Mode MODES[] = {
    {
        .index = CLEAN, 
        .clean_or_analyze = clean_data,
        .commands = {
            {
                .name = "To Lowercase",
                .execute_command = to_lowercase,
                .report_results = report_cleaned,
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            },
            {
                .name = "Remove Special Characters",
                .execute_command = remove_special_char,
                .report_results = report_cleaned,
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            },
            {
                .name = "Remove Numbers",
                .execute_command = remove_numbers,
                .report_results = report_cleaned,
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            },
            {
                .name = "Clean Whitespace",
                .execute_command = clean_whitespace,
                .report_results = report_cleaned,
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            },
            {
                .name = "Remove Stopwords",
                .execute_command = remove_stopwords,
                .report_results = report_cleaned,
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            },
            {
                .name = "All",
                .execute_command = clean_all,
                .report_results = report_cleaned, 
                .print_results = print_cleaned,
                .usedAddInt = 0,
                .usedAddStr = 0,
                .fileSuffix = ""
            }
        }
    }, 
    {
        .index = ANALYZE_SINGLE, 
        .clean_or_analyze = analyze_data__single,
        .commands = {
            {
                .name = "Word Count",
                .execute_command = get_word_count,
                .report_results = report_token_frequency,
                .print_results = print_token_frequency,
                .fileSuffix = "wcount",
                .usedAddInt = 0,
                .usedAddStr = 0,
            },
            {
                .name = "N-gram Count",
                .execute_command = get_ngram_count,
                .report_results = report_token_frequency,
                .print_results = print_token_frequency,
                .fileSuffix = "ngram",
                .usedAddInt = 0,
                .usedAddStr = 0,
            },
            {
                .name = "Concordance",
                .execute_command = get_concordance,
                .report_results = report_concordance,
                .print_results = print_concordance,
                .fileSuffix = "concord",
                .usedAddInt = 0,
                .usedAddStr = 0,
            }
        }
    },
    {
        .index = ANALYZE_MULTI, 
        .clean_or_analyze = analyze_data__multi,
        .commands = {
            {
                .name = "tf-idf",
                .execute_command = get_tfidf,
                .report_results = report_tfidf,
                .print_results = print_token_frequency,
                .fileSuffix = "tfidf",
                .usedAddInt = 0,
                .usedAddStr = 0,
            },
            {
                .name = "Document similarity",
                // .execute_command = get_document_similarity,
                // .report_results = report_document_similarity,
                .fileSuffix = "",
                .usedAddInt = 0,
                .usedAddStr = 0,
            }
        }
    }
};

#define MAX_ANALYZER_OPTIONS 3

void initialize_summary(Summary *summary)
{
    initialize_metadata(summary->metadata);
    summary->infile = NULL;
    summary->inData = NULL;
    summary->tokenList = NULL;
    summary->corpus = NULL;
    summary->corpusData = NULL;
    summary->corpusTokens = NULL;
    summary->outData = NULL;
    summary->outfile = NULL;
}

void set_mode(Summary *summary, ModeIndex mode)
{
    summary->mode = MODES[mode];
}

void set_infile(Summary *summary, Config config, char *filename)
{
    char completeFilename[MAX_CHAR];

    if(summary->mode.index == CLEAN)
        strcpy(completeFilename, config.rawDocumentPath);
    else
        strcpy(completeFilename, config.cleanedDocumentPath);

    strcat(completeFilename, "/");
    strcat(completeFilename, filename);
    summary->infile = fopen(completeFilename, "r");
    strcpy(summary->infilename, filename);
}

bool __check_if_txt_file(char *filename)
{
    int length = strlen(filename) - 4;
    char *fileExtension = filename + length;

    return strcmp(fileExtension, ".txt") == 0;
}

void initialize_corpus(Summary *summary, Config config) 
{
    char *corpusPath = config.cleanedDocumentPath;

    int filesRead = 0;
    FILE **corpus = malloc(sizeof(FILE*));

    DIR *corpusDir = opendir(corpusPath);
    struct dirent *entity = readdir(corpusDir);

    FILE *tempFile;
    int numFile = 1;

    while(entity != NULL && filesRead < config.numDocs) {
        if(__check_if_txt_file(entity->d_name)) {
            char tempName[MAX_CHAR] = "";
            
            strcpy(tempName, corpusPath);
            strcat(tempName, "/");
            strcat(tempName, entity->d_name);

            tempFile = fopen(tempName, "r");

            if(filesRead >= numFile) {
                numFile *= 4;
                corpus = realloc(corpus, sizeof(FILE*) * numFile);
            }

            corpus[filesRead] = tempFile;
            
            filesRead++;
        }

        entity = readdir(corpusDir);
    }

    corpus[filesRead] = NULL;
    summary->corpus = corpus;
}

void set_outfile(Summary *summary, Config config, char *filename)
{
    char completeFilename[MAX_CHAR];

    if(summary->mode.index == CLEAN)
        strcpy(completeFilename, config.cleanedDocumentPath);
    else
        strcpy(completeFilename, config.analysisOutputPath);

    strcat(completeFilename, "/");
    strcat(completeFilename, filename);
    summary->outfile = fopen(completeFilename, "w");
}

void execute_summary(Summary *summary)
{
    summary->mode.clean_or_analyze(summary);
}

void set_option(Summary *summary, Config config, int rawInput)
{
    summary->option = rawInput;
}

void set_add_str(Summary *summary, char *addStr)
{
    int option = summary->option;
    Command command = summary->mode.commands[option];
    
    int strUsed = command.usedAddStr;
    int len = strlen(addStr);

    strcpy(summary->addOpts.s[strUsed], addStr);
    summary->addOpts.s[strUsed][len] = '\0';
    summary->mode.commands[option].usedAddStr++;
}

void set_add_int(Summary *summary, int addInt)
{
    int option = summary->option;
    Command command = summary->mode.commands[option];
    
    int intUsed = command.usedAddInt;

    summary->addOpts.i[intUsed] = addInt;
    summary->mode.commands[option].usedAddInt++;
}

void destroy_summary(Summary *summary)
{
    if(summary->infile != NULL) {
        fclose(summary->infile);
        summary->infile = NULL;
    }

    if(summary->outfile != NULL) {
        fclose(summary->outfile);
        summary->outfile = NULL;
    }

    if(summary->inData != NULL) {
        free(summary->inData);
        summary->inData = NULL;
    }

    if(summary->outData != NULL) {
        free(summary->outData);
        summary->outData = NULL;
    }
    
    delete_metadata(summary->metadata);
    delete_token_strings(summary->tokenList);

    if(summary->tokenList != NULL) {
        destroy_tokenList(summary->tokenList);
        summary->tokenList = NULL;
    }
}
