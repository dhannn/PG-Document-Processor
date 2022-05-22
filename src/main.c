#include "pgdocs.h"
#include "deps/internals.h"
#include <ctype.h>
#include <string.h>

const AnalyzerOption ANALYZER_OPTIONS[] = {
    {"Word Count", get_word_count, report_token_frequency, "_wcount"},
    {"N-Gram Count", get_ngram_count, report_token_frequency, "_ngram"},
    {"Concordance", get_concordance, report_concordance, "_concordance"}
};

int main()
{
    Summary summary;
    Config config;
    config.numChar = 0;

    printf("Choose a mode:\n");
    printf("  [1] Cleaning\n");
    printf("  [2] Analyis\n\n");

    int i; 
    scanf("%d", &i);

    set_mode(&summary, i - 1);

    printf("Choose an option:\n");
    for(int i = 0; i < MAX_ANALYZER_OPTIONS; i++) {
        printf("  [%d] %s\n", i + 1, ANALYZER_OPTIONS[i].name);
    }
    scanf("%d", &i);

    set_options(&summary, config, i);

    summary.inFile = fopen("sample_files/cleaned_files/d.txt", "r");
    summary.outFile = fopen("meh.txt", "w");
    execute_summary(&summary, config);

    // summary.inFile = fopen("sample_files/cleaned_files/d.txt", "r");
    // summary.metadata.metadataItems[0].name = "Title";
    // summary.metadata.metadataItems[1].name = "Author";
    // summary.metadata.metadataItems[2].name = "Release Date";
    // summary.metadata.metadataItems[3].name = "Language";
    // summary.metadata.metadataItems[4].name = "Produced by";
    // summary.options = 1;
    // summary.outFile = fopen("samples_files/output/a_word_count.txt", "w");
    // read_file(&summary, config);

    // get_word_count(&summary, config);



    return 0;
}
