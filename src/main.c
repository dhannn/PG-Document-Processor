#include "pgdocs.h"
#include "deps/internals.h"
#include <ctype.h>
#include <string.h>

int main()
{
    Summary summary;
    Config config;
    config.numChar = 0;
    summary.inFile = fopen("sample_files/cleaned_files/d.txt", "r");
    summary.metadata.metadataItems[0].name = "Title";
    summary.metadata.metadataItems[1].name = "Author";
    summary.metadata.metadataItems[2].name = "Release Date";
    summary.metadata.metadataItems[3].name = "Language";
    summary.metadata.metadataItems[4].name = "Produced by";
    summary.options = 1;
    summary.outFile = fopen("samples_files/output/a_word_count.txt", "w");
    read_clean_file(&summary, config, "s");

    // remove_numbers (&summary, config);
    // remove_special (&summary, config);
    // to_lowercase (&summary, config);
    // clean_whitespace (&summary, config);

    print_tokens (summary.tokenList);


    //get_word_count(&summary, config);



    return 0;
}
