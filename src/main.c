/*******************************************************************************
 * 
 * This is to certify that this project is our own work, based on our personal
 * efforts in studying and applying the concepts learned. We have constructed the
 * functions and their respective algorithms and corresponding code by ourselves.
 * The program was run, tested, and debugged by our own efforts. We further 
 * certify that we have not copied in part or whole or otherwise plagiarized 
 * the work of other students and/or persons.
 * 
 *                                      Daniel Ramos,   DLSU ID# 1213136
 *                                      Gwen Roco,      DLSU ID# 12115207
 * 
 *******************************************************************************/

/*******************************************************************************
 * 
 * FILE             main.c
 * LAST MODIFIED    06-
 * AUTHORS          Ramos, Daniel III
 *                  Roco, Gwen
 * 
 * DESCRIPTION
 *      The main program logic that uses the modules.
 * 
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pgdocs.h"

int main()
{
    ActiveScreen activeScreen;
    Summary summary;
    Config config;

    initialize_screens(&activeScreen);
    initialize_config(&activeScreen, &config);

    go_to_screen(&activeScreen, &summary, MAIN_MENU);

    do {
        int index = 0;

        if(activeScreen.current->get_input == get_choice)
            index = activeScreen.choice - 1;
        
        activeScreen.current->options[index].do_option(
            &activeScreen, 
            &summary, 
            &config);
        
    } while(!check_if_exit(activeScreen.screens, &activeScreen));

    return 0;
}
