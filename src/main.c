#include <stdio.h>
#include <string.h>
#include "pgdocs.h"

int main()
{
    Screen screens[MAX_SCREENS];
    ActiveScreen activeScreen;
    // Summary summary;
    // Config config;

    initialize_screens(screens);
    go_to_screen(screens, &activeScreen, ENTER_PATH_MENU);
    display_screen(&activeScreen);

    return 0;
}
