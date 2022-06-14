#include "pgdocs.h"
#include "deps/internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
                // .execute_command = get_tfidf,
                // .report_results = report_tfidf,
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

    // if(command.addStrNeeded == 0)
    //     return;
    
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
    fclose(summary->infile);
    fclose(summary->outfile);
    free(summary->inData);
    free(summary->outData);
    
    delete_metadata(summary->metadata);
    delete_token_strings(summary->tokenList);
    destroy_tokenList(summary->tokenList);
}
