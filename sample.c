#include <ncurses/ncurses.h>

int main()
{
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    printw("Hello world%c%c\n", '!', '!');
    int ch = getch();

    int y = 1;

    while(ch != 'q'){
        if(ch == KEY_DOWN) {
            y++;
        } else if(ch == KEY_UP) {
            y--;
        }
        
        move(y, 0);
        printw("%d\n", ch);
        ch = getch();
        refresh();
    }

    endwin();

    return 0;
}
