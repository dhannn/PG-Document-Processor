#include <stdio.h>
#include "pgdocs.h"

int main()
{
    Screen screens[MAX_SCREENS];

    initialize_screens(screens);

    for(int i = 0; i < MAX_SCREENS; i++) {
        for(int j = 0; j < 2; j++)
            printf("%s\n", screens[i].header[j]);
        printf("\n");
    }
}
