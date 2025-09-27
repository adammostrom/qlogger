#include "header.h"


using namespace std;
using namespace std::chrono;

string get_date();

int main() {

    string date = get_date();
    cout << date;

    initscr();                // Start ncurses mode
    cbreak();                 // Disable line buffering
    noecho();                 // Don't echo typed characters
    keypad(stdscr, TRUE);     // Enable special keys (arrows, etc.)

    refresh();

    int ch;
    int x = 0, y = 1;         // initialize coords
    printw("%d, %d", x,y);

    while ((ch = getch()) != 'q') {  // loop until q pressed
        string date = get_date();
        int size = date.length();

        if(ch == '\n' || ch == KEY_ENTER){
            y++;
            x = 0; // reset x to start of line
            move(y, x);
            
            //printw("%s > ", date.c_str()); // use c_str() here
        }
        if(ch == KEY_UP){
            y = (y > 0 ? y - 1 : 0);
            move(y, x);
            refresh();
        }
        else if(ch == KEY_DOWN){
            move(y++, x);
            refresh();
        }
        else if(ch == KEY_RIGHT){
            move(y, x++);
            refresh();
        }
        else if(ch == KEY_LEFT){
            //x = (x > 0 ? x - 1 : 0);
            move(y, x--);
            refresh();
        }
        else {
            printw("%c", ch);
            refresh();
            x++;                  // move forward each time
            move(y, x);           // move cursor to (y,x)
        }
            

    }

    endwin();                 // Always restore terminal before exiting
    return 0;
}




string get_date(){
    auto now_f = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_f); // Now first

    ostringstream oss;
    oss << std::put_time(localtime(&now_c), "%Y-%m-%d - %H:%M:%S");
    return oss.str();
}