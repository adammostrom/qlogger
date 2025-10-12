class Cursor {
    int pos_x;
    int pos_y;
    WINDOW* win;

public:
    Cursor(WINDOW* w, int x = 0, int y = 0)
        : pos_x(x), pos_y(y), win(w) {
        wmove(win, y, x);
        wrefresh(win);
    }

    int get_x() const { return pos_x; }
    int get_y() const { return pos_y; }

    void set_x(int x) { pos_x = x; update(); }
    void set_y(int y) { pos_y = y; update(); }

    void move_left()  { if (pos_x > 0) pos_x--; update(); }
    void move_right() { pos_x++; update(); }
    void move_up()    { if (pos_y > 0) pos_y--; update(); }
    void move_down()  { pos_y++; update(); }

    void update() {
        wmove(win, pos_y, pos_x);
        wrefresh(win);
    }
};