

class Cursor {
    int pos_x;
    int pos_y;

public:
    Cursor(int x = 0, int y = 0) : pos_x(x), pos_y(y) {} // constructor

    int get_x() const { return pos_x; }
    int get_y() const { return pos_y; }

    void set_x(int x) { pos_x = x; }
    void set_y(int y) { pos_y = y; }

    void move_left()  { if (pos_x > 0) pos_x--; }
    void move_right() { pos_x++; }
    void move_up()    { if (pos_y > 0) pos_y--; }
    void move_down()  { pos_y++; }
};