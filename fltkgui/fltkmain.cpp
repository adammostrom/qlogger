#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <fstream>
#include <vector>
#include <FL/fl_draw.H>
#include <iostream>

#define WINDOW_HEIGHT 1200
#define WINDOW_WIDTH 1500
#define TEXT_BOX_HEIGHT 30
#define INPUT_TEXT_BUFFER_SPACE 80
#define MARGIN 25
#define ENTITY_SPACING 40
#define BOX_MARGINS 50
#define BOX_START_X 20


struct Field { const char* label; };
std::vector<Fl_Input*> inputs;  // global list of inputs

std::vector<int> box_heights;

void start_parameters(Fl_Window* win, const std::vector<Field> &fields);
void button_box(Fl_Window* win, int start_y);


void save_cb(Fl_Widget*, void*) {
    std::ofstream file("log.csv", std::ios::app);
    for (auto* input : inputs) {
        file << input->value() << ",";
    }
    file << "\n";
    file.close();
}

int main() {
    Fl_Window win(WINDOW_WIDTH, WINDOW_HEIGHT, "Logger");



    std::vector<Field> fields = {
        {"Date:"}, {"Device ID:"}, {"Operator:"}, {"Comment:"}, {"TEST:"}, {"TEST:"},
    };
    
    //MENU_WINDOW(); // A temporary popup window
    
    start_parameters(&win, fields);

    int freespace = BOX_MARGINS;
    for(int &box_size : box_heights){
        freespace += box_size;
    }
    button_box(&win, freespace);
    

    win.end();
    win.show();
    return Fl::run();
}



// TODO: Make Struct for a box, with parameters: x, y, width, height, label, columns
void start_parameters(Fl_Window* win, const std::vector<Field>& fields){

    int box_start_x = BOX_START_X;
    int box_start_y = ENTITY_SPACING;
    const int cols = 2;
    int col_width = ((WINDOW_WIDTH - (MARGIN * (cols + 1))) - box_start_x) / cols;
    int y = MARGIN;

    int rows = (fields.size() + (cols - 1)) / cols;  // ceil division
    int group_box_height =  rows * (TEXT_BOX_HEIGHT + MARGIN) + MARGIN;

    
    win->begin();
    // --- Section 1: Device Info ---
    Fl_Group* device_group = new Fl_Group(box_start_x, box_start_y, WINDOW_WIDTH - MARGIN, group_box_height, "Device Info");
    device_group->box(FL_UP_BOX); // visualize bounds
    device_group->align(FL_ALIGN_TOP_LEFT);
    device_group->labelsize(20);
    device_group->labelfont(5);
    device_group->labelcolor(FL_BLACK);
    
    
    for (size_t i = 0; i < fields.size(); ++i) {
        int col = i % cols;
        int row = i / cols;

        int x = box_start_x + MARGIN  + col * (col_width + MARGIN);
        y = box_start_y + MARGIN + row * (TEXT_BOX_HEIGHT + MARGIN);

        auto* input = new Fl_Input(x, y, col_width, TEXT_BOX_HEIGHT, fields[i].label);
        input->align(FL_ALIGN_TOP_LEFT);  // label above box
        inputs.push_back(input);

    }

    box_heights.push_back(group_box_height);
    device_group->end();  // IMPORTANT!
    win->end();
}

void button_box(Fl_Window* win, int start_y){
    
    win->begin();
    // --- Section 1: Device Info ---

    // POS X, POS Y, WIDTH, HEIGHT
    Fl_Group* utility_buttons = new Fl_Group(BOX_START_X, start_y + ENTITY_SPACING,WINDOW_WIDTH -  MARGIN,(TEXT_BOX_HEIGHT + MARGIN) * 2, "Utility");
    utility_buttons->box(FL_UP_BOX); // visualize bounds
    utility_buttons->align(FL_ALIGN_TOP_LEFT);
    utility_buttons->labelsize(20);
    utility_buttons->labelfont(5);
    utility_buttons->labelcolor(FL_BLACK);


    Fl_Button btn(WINDOW_HEIGHT - MARGIN, MARGIN, 100, 40, "Save");

    btn.callback(save_cb);

    utility_buttons->end();  // IMPORTANT!
    win->end();
}

