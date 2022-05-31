#include "pgdocs.h"
#include <stdbool.h>

void go_to_screen(Screen *screens[], ActiveScreen *active)
{
    int option = active->nInput - 1;
    
    active->screen = &screens[option];
}

void get_int(ActiveScreen *active)
{
    scanf ("%d", &active->nInput);
}

void get_str(ActiveScreen *active)
{
    scanf ("%s", active->strInput);
}

void print_metadata(char *metadataName[], char *metadata[], int maxMetadata)
{
    printf ("************************************************\n");
		
    for (int i = 0; i < maxMetadata; i++){
        printf ("%s: %s\n", metadataName[i], metadata[i]);
     }

    printf ("************************************************\n");
}