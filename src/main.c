#include <stdio.h>
#include <string.h>
#include "pgdocs.h"

int main()
{
    Screen screens[MAX_SCREENS];
    char *metadataItems[] = {"Song Title", "Album", "Artist", "Release Date"};
    char *metadata[] = {"All Too Well (Taylor's Version) (10-Minute Version) (From The Vault)", "Red (Taylor's Version)", "Taylor Swift", "2021"};

    initialize_screens(screens);
    print_metadata(metadataItems, metadata, 4);

    return 000000000;
}
