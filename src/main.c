#include <stdio.h>
#include <string.h>
#include "pgdocs.h"

int main()
{
    ActiveScreen activeScreen;
    Summary summary;
    Config config;

    initialize_screens(&activeScreen);
    initialize_config(&activeScreen, &config);

    go_to_screen(&activeScreen, MAIN_MENU);
    do {
        display_screen(&activeScreen, &summary);
        activeScreen.current->get_input(&activeScreen);

        int index = 0;
        if(activeScreen.current->get_input == get_choice)
            index = activeScreen.choice - 1;
        
        activeScreen.current->options[index].do_option(&activeScreen, &summary, &config);
    } while(!check_if_exit(activeScreen.screens, &activeScreen));

    //get_word_count(&summary, config);
    
    return 0;
}
