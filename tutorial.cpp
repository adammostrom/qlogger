#include <ncurses.h>

int main() {
    initscr();                // Start ncurses mode
    cbreak();                 // Disable line buffering
    noecho();                 // Don't echo typed characters
    keypad(stdscr, TRUE);     // Enable special keys (arrows, etc.)

    printw("Hello World. Press q to quit.");
    refresh();

    int ch;
    int x = 0, y = 1;         // initialize coords

    while ((ch = getch()) != 'q') {  // loop until q pressed
        printw("%c", ch);
        refresh();
        x++;                  // move forward each time
        move(y, x);           // move cursor to (y,x)
        if(ch == '\n' || ch == KEY_ENTER){
            y++;
            x = 0; // reset x to start of line
            move(y, x);
            printw("> ");
        }
    }

    endwin();                 // Always restore terminal before exiting
    return 0;
}


int get_date(){
    
}