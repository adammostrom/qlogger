#include "header.h"
#include "cursor.cpp"


using namespace std;
using namespace std::chrono;

string get_date();
void key_input(int ch, int x, int y);
void init_window(int x, int y);
void exit_window(int x, int y);
void popup(const char* msg);

int main() {

    initscr();                // Start ncurses mode
    cbreak();                 // Disable line buffering
    
    noecho();                 // Don't echo typed characters
    keypad(stdscr, TRUE);     // Enable special keys (arrows, etc.)

    std::vector<std::string> buffer;
    buffer.push_back("");  // start with one empty line

    refresh();

    int x = 0, y = 1;         // initialize coords

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Fore and background colors.

    attron(COLOR_PAIR(1));

    int ch;

    Cursor cursor(0, 0);

    // Main loop
    while (true) {  // loop until q pressed
        string date = get_date();
        int size = date.length();

        ch = getch();

        if(ch == 'q'){
            popup("Hello");
        }
            switch(ch){
                case KEY_UP: cursor.move_up(); break;
                case KEY_DOWN: cursor.move_down(); break;
                case KEY_LEFT: cursor.move_left(); break;
                case KEY_RIGHT: cursor.move_right(); break;
                case '\n':     // ENTER
                case KEY_ENTER:
                    printw("\n");              // ✅ print real newline
                    cursor.move_down();
                    cursor.set_x(0);
                    break;
                default: 
                    printw("%c", ch);
                    cursor.move_right();
            } 
    
            move(cursor.get_y(), cursor.get_x());
            refresh();
            
        refresh();

    }

            
    attroff(COLOR_PAIR(1));

    endwin();                 // Always restore terminal before exiting
    return 0;
}

/* 
void key_input(int ch, int x, int y){
        if(ch == '\n' || ch == KEY_ENTER){
                y++;
                x = 0; // reset x to start of line
                //printw("%s > ", date.c_str()); // use c_str() here
            }
            else if(ch == KEY_UP){
                y = (y > 0 ? y - 1 : 0);
            }
            else if(ch == KEY_DOWN){
                y++;
            }
            else if(ch == KEY_RIGHT){
                x++;
            }
            else if(ch == KEY_LEFT){
                if(x > 0) x--;
            }
            else {
                printw("%c", ch);
                x++;                  // move forward each time
            }
        move(y, x);           // move cursor to (y,x)
        refresh();
} */

void popup(const char* message) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);   // get screen size

    int height = 7, width = 30;
    int starty = (rows - height) / 2;
    int startx = (cols - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    mvwprintw(win, 2, 2, "%s", message);
    mvwprintw(win, height - 2, 2, "Press any key...");
    wrefresh(win);

    getch();        // wait for key
    delwin(win);    // delete window

    // redraw main screen so popup disappears
    touchwin(stdscr);
    refresh();
}

void exit_window(int x, int y){
    WINDOW *logwin = newwin(10, 30, 5, 5); // height, width, y, x
    box(logwin, 0, 0);
    wattron(logwin, COLOR_PAIR(1));
    mvwprintw(logwin, 1, 1, "Are you sure you want to exit?");
    wattroff(logwin, COLOR_PAIR(1));
    wrefresh(logwin);

}

void init_window(int x, int y){
    // Define window size and position
    int win_height = 10, win_width = 30, win_starty = 0, win_startx = 50;
    WINDOW* win = newwin(win_height, win_width, win_starty, win_startx);
    box(win, 0, 0); // Draw border
    wprintw(win, "Window!");
    wrefresh(win); // refresh the window

    // To get window position and size, use the variables above (win_startx, win_starty, win_width, win_height)
    // To avoid collision, ensure your text (x, y) stays outside the window's area:
    // For example, don't print text where: x >= win_startx && x < win_startx + win_width
    // and y >= win_starty && y < win_starty + win_height

    // Example: move text start position if it would collide
    if (x >= win_startx && x < win_startx + win_width &&
        y >= win_starty && y < win_starty + win_height) {
        x = 0;
        y = win_starty + win_height; // move below the window
    }

    // When done
    delwin(win); // delete window when done
}


string get_date(){
    auto now_f = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_f); // Now first

    ostringstream oss;
    oss << std::put_time(localtime(&now_c), "%Y-%m-%d - %H:%M:%S");
    return oss.str();
}


