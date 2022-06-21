/*******************************************************************************
 * 
 * This is to certify that this project is our own work, based on our personal
 * efforts in studying and applying the concepts learned. We have constructed the
 * functions and their respective algorithms and corresponding code by ourselves.
 * The program was run, tested, and debugged by our own efforts. We further 
 * certify that we have not copied in part or whole or otherwise plagiarized 
 * the work of other students and/or persons.
 * 
 *                                      Daniel Ramos,   DLSU ID# 12131369
 *                                      Gwen Roco,      DLSU ID# 12115207
 * 
 *******************************************************************************/

/*******************************************************************************
 * 
 * FILE             main.c
 * LAST MODIFIED    06-15-2022
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

// #define DEBUG 1

int main()
{
    ActiveScreen activeScreen;
    Summary summary;
    Config config;

    initialize_screens(&activeScreen);
    initialize_config(&activeScreen, &config);
    initialize_summary(&summary);

    go_to_screen(&activeScreen, &summary, MAIN_MENU);

    do {
        int index = 0;
        Screen *current = activeScreen.current;

        if(current->get_input == get_choice)
            index = activeScreen.choice - 1;
        
        void (*do_option)(ActiveScreen*, Summary*, Config*);
        do_option = current->options[index].do_option;

        if(do_option != NULL)
            do_option(&activeScreen, &summary, &config);
        
    } while(!check_if_exit(activeScreen.screens, &activeScreen));

    destroy_summary(&summary);

    return 0;
}
