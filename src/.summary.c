#include "pgdocs.h"
#include "deps/internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

unsigned int _convert_multiselect_options(int rawInput);

void set_mode(Summary *summary, Mode mode)
{
    summary->mode = mode;
}

void execute_summary(Summary *summary, Config config)
{
    summary->metadata = initialize_metadata();
    read_file(summary, config);
    tokenize_string(summary->inData, summary->mode == CLEAN);

    if(summary->mode == CLEAN)
        printf("%d", summary->mode);
    else
        analyze_data(summary, config);
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
    delete_metadata(&summary->metadata);
}
