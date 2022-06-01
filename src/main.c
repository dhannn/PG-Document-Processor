#include <stdio.h>
#include <string.h>
#include "pgdocs.h"

int main()
{
    Screen screens[MAX_SCREENS];

    initialize_screens(screens);

    for(int i = 0; i < MAX_SCREENS; i++) {
        for(int j = 0; j < 2; j++)
            printf("%s\n", screens[i].header[j]);

        printf("\n");

        for(int k = 0; k < MAX_OPTIONS; k++) {
            if(strcmp(screens[i].options[k].optionName, "") != 0)
                printf("\t[%d] %s\n", k + 1, screens[i].options[k].optionName);
        }

        printf("\n\t%s\n", screens[i].prompt);

        printf("\t>> ");
    }
}
