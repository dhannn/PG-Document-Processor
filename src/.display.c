#include <stdio.h>

#define MAX_WIDTH 85
#define MAX_HEIGHT 35

// const char **TITLE = {
//     "█▀█ █▀▀   █▀▄ █▀█ █▀▀ █░█ █▀▄▀█ █▀▀ █▄░█ ▀█▀   █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀ █▀█ █▀█",
//     "█▀▀ █▄█   █▄▀ █▄█ █▄▄ █▄█ █░▀░█ ██▄ █░▀█ ░█░   █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█ █▄█ █▀▄"
// };

// const char **DATA_CLEANER = {
//     "█▀▄ ▄▀█ ▀█▀ ▄▀█   █▀▀ █░░ █▀▀ ▄▀█ █▄░█ █▀▀ █▀█", 
//     "█▄▀ █▀█ ░█░ █▀█   █▄▄ █▄▄ ██▄ █▀█ █░▀█ ██▄ █▀▄"
// };

// const char **DATA_CLEANER = {
//     "█▀▄ ▄▀█ ▀█▀ ▄▀█   ▄▀█ █▄░█ ▄▀█ █░░ █▄█ ▀█ █▀▀ █▀█", 
//     "█▄▀ █▀█ ░█░ █▀█   █▀█ █░▀█ █▀█ █▄▄ ░█░ █▄ ██▄ █▀▄"
// };

typedef struct {
    int index;
    char *option;
    void (*show_next_screen)(void*);
} ScreenOption;

typedef struct {
    int n;
    char *input;
    ScreenOption options[];
} Screen;

Screen *initialize_screens();

Screen *initialize_screens()
{
    return NULL;
}
