#include <stdio.h>
#include <string.h>
#include "pgdocs.h"

int main()
{
    ActiveScreen activeScreen;
    Summary summary;
    Config config;

    initialize_screens(&activeScreen);
    config.isInitialized = false;

    go_to_screen(&activeScreen, MAIN_MENU);
    do {
        display_screen(&activeScreen);
        activeScreen.current->get_input(&activeScreen);

        int index = 0;
        if(activeScreen.current->get_input == get_int)
            index = activeScreen.nInput - 1;
        
        activeScreen.current->options[index].do_option(&activeScreen, &summary, &config);
    } while(!check_if_exit(activeScreen.current, &activeScreen));
    display_screen(&activeScreen);

    return 0;
}
