#ifndef PG_DOCS_H
#define PG_DOCS_H

#include <stdbool.h>
#define MAX_CHAR 1024

#define MAX_OPTIONS 10
#define MAX_SCREENS 12

typedef struct {
    char optionName[MAX_CHAR];
    void (*do_option)(void*, void*, void*);
} ScreenOption;

typedef struct _activeScreen ActiveScreen;

typedef struct { 
    char **header;
    char *prompt;
    ScreenOption options[MAX_OPTIONS];
    void (*get_input)(ActiveScreen*);
    int backIndex;
} Screen;

struct _activeScreen {
    Screen *screen;
    int nInput;
    char strInput[MAX_CHAR];
};

void initialize_screens(Screen screens[]);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(Screen *screens[], ActiveScreen *active);
void get_int(ActiveScreen *active);
void get_str(ActiveScreen *active);
void print_metadata(char *metadataName, char *metadata);

#endif
