#include "pgdocs.h"
#include "deps/internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

Mode MODES[] = {
    {.index = CLEAN, .clean_or_analyze = clean_data}, 
    {.index = ANALYZE_SINGLE, .clean_or_analyze = analyze_data__single},
    {.index = ANALYZE_MULTI, .clean_or_analyze = analyze_data__multi}
};

#define MAX_ANALYZER_OPTIONS 3

unsigned int _convert_multiselect_options(int rawInput);

void set_mode(Summary *summary, ModeIndex mode)
{
    summary->mode = MODES[mode];
}

void execute_summary(Summary *summary, Config config)
{
    initialize_metadata(summary->metadata);
    read_file(summary, config);
    tokenize_string(summary->inData, summary->mode.index == CLEAN);

    summary->mode.clean_or_analyze(summary, config);
}

void set_options(Summary *summary, Config config, int rawInput)
{
    if(config.multiSelect == false) {
        summary->options = (int)pow(2, rawInput - 1);
        return;
    }

    summary->options = _convert_multiselect_options(rawInput);
}

unsigned int _convert_multiselect_options(int rawInput)
{
    // a lookup table to prevent duplicates
    bool isEntered[MAX_ANALYZER_OPTIONS];

    for(int i = 0; i < MAX_ANALYZER_OPTIONS; i++) 
        isEntered[i] = false;

    
    unsigned int options = 0;
    while(rawInput > 0) {
        // gets rightmost digit
        int currDigit = rawInput % 10;

        // only adds the option to bitfield only when not added
        if(isEntered[currDigit] == false) {
            options += (int)pow(2, currDigit - 1);
            isEntered[currDigit] = true;
        }

        // removes rightmost digit
        rawInput /= 10;
    }

    return options;
}

void destroy_summary(Summary *summary)
{
    delete_token_strings(summary->tokenList);
    destroy_tokenList(summary->tokenList);
}
