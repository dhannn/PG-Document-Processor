#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgdocs.h"

#define MAX_WIDTH       85
#define MAX_HEIGHT      35
#define MAX_HEADER_ROW  2

#define MAX_SCREENS     4

#define WSTRLEN_COEFFFICIENT    (0.350649351)

const char *TITLE[] = {
    "█▀█ █▀▀   █▀▄ █▀█ █▀▀ █░█ █▀▄▀█ █▀▀ █▄░█ ▀█▀   █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀ █▀█ █▀█",
    "█▀▀ █▄█   █▄▀ █▄█ █▄▄ █▄█ █░▀░█ ██▄ █░▀█ ░█░   █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█ █▄█ █▀▄"
};

const char *DATA_CLEANER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   █▀▀ █░░ █▀▀ ▄▀█ █▄░█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▄▄ █▄▄ ██▄ █▀█ █░▀█ ██▄ █▀▄"
};

const char *DATA_ANALYZER[] = {
    "█▀▄ ▄▀█ ▀█▀ ▄▀█   ▄▀█ █▄░█ ▄▀█ █░░ █▄█ ▀█ █▀▀ █▀█", 
    "█▄▀ █▀█ ░█░ █▀█   █▀█ █░▀█ █▀█ █▄▄ ░█░ █▄ ██▄ █▀▄"
};

const char **HEADERS[] = {TITLE, DATA_CLEANER, DATA_ANALYZER};

Screen *initialize_screens();
void destroy_screens();

Screen *initialize_screens()
{
    Screen *screen = malloc(sizeof(Screen) * MAX_SCREENS);
    
    for(int i = 0; i < MAX_SCREENS; i++) {
        const char **headerRow = HEADERS[i];
        screen[i].header = headerRow;
    }

    return screen;
}
