#ifndef PG_DOCS_H
#define PG_DOCS_H
#include <stdbool.h>

#define MAX_CHAR 1024

typedef struct _screen Screen;

typedef struct {
    char *optionName;
    Screen *resultingScreen;
    bool pointsToCommand;
} ScreenOption;

struct _screen { 
    char **header;
    char *prompt;
    ScreenOption *options;
    void (*get_input)(ActiveScreen);
    int backIndex;
};

typedef struct {
    Screen *screen;
    int nInput;
    char strInput[MAX_CHAR];
} ActiveScreen;

void initialize_screens(Screen screens[]);
void activate_screen(ActiveScreen* active, Screen screens[]);
void __validate_screen_option(ActiveScreen *active);
void go_to_screen(Screen *screens[], ActiveScreen *active);
void get_int(ActiveScreen *active);
void get_str(ActiveScreen *active);
void print_metadata(char *metadataName[], char *metadata[], int maxMetadata);

#endif
