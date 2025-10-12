#include "header.h"
#include "cursor.cpp"


/*
Globals
*/
#define INFO_BOX_HEIGHT 10
#define INFO_BOX_WIDTH 80

/*
Structures
*/

struct Timestruct {
    std::string date;
    std::string time;
};



std::string get_date();
void key_input(int ch, int x, int y);
int init_window();
bool exit_window();
void popup(const char* msg);
void info_box();
Timestruct get_date_struct();

int main() {

    initscr();                // Start ncurses mode
    //cbreak();                 // Disable line buffering: “Give me each keystroke immediately — including control keys.”
    raw();    
    noecho();                 // Don't echo typed characters
    keypad(stdscr, TRUE);     // Enable special keys (arrows, etc.)
    
    std::vector<std::string> buffer;
    buffer.push_back("");  // start with one empty line
    
    refresh();

    
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Fore and background colors.
    
    attron(COLOR_PAIR(1));
    
    int ch;
    
    std::string date = get_date();
    
    // Main loop
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Load the info box area
    info_box();
    
    // === TEXT AREA BELOW ===
    int text_height = rows - INFO_BOX_HEIGHT;
    WINDOW* text_win = newwin(text_height, cols, INFO_BOX_HEIGHT, 0);
    scrollok(text_win, TRUE);
    keypad(text_win, TRUE);
    
    // move cursor to text area start
    Cursor cursor(text_win, 0, 0);
    wrefresh(text_win);
    
    while (true) {  // loop until q pressed
        
        
        // Starts cursor in the text window
        ch = wgetch(text_win);
        
        // Refreshes while/if terminal is changing size (repaints).
        info_box();
        
        
        if(ch == ('q' & 0x1F)){
            if(exit_window()){
                break;
            }
        }
        switch(ch){
            case KEY_UP: cursor.move_up(); break;
            case KEY_DOWN: cursor.move_down(); break;
            case KEY_LEFT: cursor.move_left(); break;
            case KEY_RIGHT: cursor.move_right(); break;
            case '\n':     // ENTER
            case KEY_ENTER:
                waddch(text_win, '\n');   // ✅ write to text window
                cursor.move_down();
                cursor.set_x(0);
                break;
            default:
            if (isprint(ch)) {
                waddch(text_win, ch); // ✅ write to text window
                cursor.move_right();
            }
            break;
        }

    // ✅ move cursor within text_win, not stdscr
    wmove(text_win, cursor.get_y(), cursor.get_x());
    wrefresh(text_win);

    // ✅ keep info box visible (if something overwrote it)
    //wrefresh(info_win);

    }

            
    attroff(COLOR_PAIR(1));
    endwin();   // ✅ cleanup ncurses properly here
    return 0;
}

void info_box(){

    std::string date = get_date();

    Timestruct ts = get_date_struct();

    // === INFO BOX ===

    WINDOW* info_win = newwin(INFO_BOX_HEIGHT, INFO_BOX_WIDTH, 0, 0);
    box(info_win, 0, 0);
    mvwprintw(info_win, 1, 2, "DEVICE ID: 12345");
    mvwprintw(info_win, 2, 2, "DATE: %s", ts.date.c_str());


    // Version readouts
    mvwprintw(info_win, 1, INFO_BOX_WIDTH / 2, "SOFTWARE VERSIONS");
    mvwprintw(info_win, 2, INFO_BOX_WIDTH / 2, "RED");
    mvwprintw(info_win, 3, INFO_BOX_WIDTH / 2, "GPP: 1234");
    mvwprintw(info_win, 4, INFO_BOX_WIDTH / 2, "FPGA: 1234");

    wrefresh(info_win);
}

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

bool exit_window() {
    std::vector<std::string> exit_lines = {"QUIT", "Are you sure you want to quit? (y/n)"};


    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Find longest line
    int max_width = 0;
    for(const auto& line : exit_lines){
        max_width = std::max(max_width, (int)line.length());
    }

    // Add padding
    int height = exit_lines.size() + 4;
    int width = max_width + 4;

    // Center it
    int starty = (rows - height) / 2;
    int startx = (cols - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    // Print lines centered inside the window
    for(int i = 0; i < (int)exit_lines.size(); i++){
        mvwprintw(win, i + 2, 2, "%s", exit_lines[i].c_str());
    }

    wrefresh(win); // ✅ ensure it’s drawn before waiting

    int ch = wgetch(win);  // get input from that popup

    bool confirm = (ch == 'y' || ch == 'Y' || ch == 'q');

    // cleanup
    delwin(win);
    touchwin(stdscr);
    refresh();

    return confirm;
}

int init_window(){
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    
    int info_height = 5;
    int info_width  = cols - 2;
    int info_starty = 0;
    int info_startx = 1;

    WINDOW* info_win = newwin(info_height, info_width, info_starty, info_startx);
    box(info_win, 0, 0);
    mvwprintw(info_win, 1, 2, "INFORMATION BOX!");
    mvwprintw(info_win, 2, 2, "DEVICE ID: 12345");
    wrefresh(info_win);

    return info_height;
}


std::string get_date(){
    auto now_f = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now_f); // Now first

    std::ostringstream oss;
    oss << std::put_time(localtime(&now_c), "%Y-%m-%d - %H:%M:%S");
    return oss.str();
}

Timestruct get_date_struct(){
    auto now_f = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now_f); // Now first

    Timestruct timestruct;
    
    std::ostringstream date_ss, time_ss;
    
    date_ss << std::put_time(localtime(&now_c), "%Y-%m-%d");
    timestruct.date = date_ss.str();


    time_ss << std::put_time(localtime(&now_c), "%H:%M");
    timestruct.time = time_ss.str();
    
    return timestruct;
}

