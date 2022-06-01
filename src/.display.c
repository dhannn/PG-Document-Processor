#include "pgdocs.h"
#include <stdbool.h>

void display_screen(ActiveScreen *active)
{
    for (int i = 0; i < 2; i++)
        printf("%s\n", active->screen->header[i]);

    printf("\n");

    for (int i = 0; i < MAX_OPTIONS; i++){
        if (strcmp(active->screen->options[i].optionName, "") != 0)
            printf("\t[%d] %s\n", i + 1, active->screen->options[i].optionName);
    }

    printf("\n\t%s\n", active->screen->prompt);
    printf("\t>> ");
}


void go_to_screen(Screen screens[], ActiveScreen *active, int index)
{   
    active->screen = &screens[index];
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