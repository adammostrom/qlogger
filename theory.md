# NCURSES THEORY


`#include <ncurses.h>`


Compile: 
`g++ main.cpp -o main -lncurses`

Its important to initialize ncurses before using it and cleaning up after:

```C

#include <ncurses.h>

int main(){
    initscr(); // Start ncurses mode
    printw("Hello World"); // Print text at current cursor
    refresh(); // Refresh to show it on screen
    getch(); // Wait for user input
    endwin(); // End ncurses mode(restore terminal)
    return 0; 
}

```

```C
    move(y, x); // Move cursor to row y col x
    mvprintw(y, x, "Hi"); // Move then print at ()
```
y = row (0 top)
x = column (0 left)


```C
    int ch = getch();  // waits for key press
```

By default, getch() blocks, to make it non-blocking:

```C
    nodelay(stdscr, TRUE);
    int ch = getch();
    if (ch == ERR) { /* no input */ }
```

- Returns an int representing the key
- For special keys (arrows, F1, etc) enable keypad:

```C
    keypad(stdscr, TRUE); // enable special keys
    noecho(); // dont echo typed chars
    cbreak(); // dont wait for enter
```
check for keys:
```C
if (ch == KEY_UP) { /* Up arrow */ }
if (ch == 'q') { /* pressed q */ }

```

## WINDOWS

```C
WINDOW* win = newwin(height, width, starty, startx);
box(win, 0, 0); // Draw border
wprintw(win, "Window!");
wrefresh(win); // refresh the window
delwin(win); // delete window when done

```

## COLORS

```C
    start_color();                    // enable colors
    init_pair(1, COLOR_RED, COLOR_BLACK); // id=1, fg=red, bg=black
    attron(COLOR_PAIR(1));            // turn on color
    printw("Red text");
    attroff(COLOR_PAIR(1));           // turn off color
```

In the windows:

```C
    wattron(win, COLOR_PAIR(1));
    wprintw(win, "Red text in window");
    wattroff(win, COLOR_PAIR(1));
```

