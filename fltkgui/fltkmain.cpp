#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include <fstream>
#include <vector>
#include <FL/fl_draw.H>
#include <iostream>

#include "params.h"


struct Box;


// STRUCTURES
struct Field { 
    std::string label;
    std::string value;
};

struct Freespace {
    int freespace = 0;

    void add(int h){
        freespace += (h + MARGIN);
    }

    int get(){
        return freespace;
    }
};


struct Box_Data_Container{
    std::vector<Box*> boxes;
    Fl_Multiline_Input* in_field;
};
/// 


// FUNCTIONS 
void start_parameters(Fl_Window* win, const std::vector<Field> &fields, const int cols);
void button_box(Fl_Window* win, int start_y);
void draw_box(Fl_Window* win, Box& box, Freespace& freespace);
void save_to_file(Fl_Widget*, void* data);
void save_cb(Fl_Widget* w, void* data) ;
///




struct Box {
    std::string name;
    std::vector<Field> fields;
    int cols;

    void draw(Fl_Window* win, Freespace& freespace){
        draw_box(win, *this, freespace);
    };
};


int main() {
    Fl_Window win(WINDOW_WIDTH, WINDOW_HEIGHT, "Logger");

    Freespace freespace{BOX_MARGINS};

    Box_Data_Container* container = new Box_Data_Container();
    
    std::vector<Field> fields = {
        {"Date:"}, {"Device ID:"}, {"Operator:"}, {"Comment:"}, {"TEST:"}, {"TEST:"},
    };
    
    //MENU_WINDOW(); // A temporary popup window
    
    
    // Create a Box with its own fields array
    Box device_box{
        "devices", fields, 2
    };
    
    Box software_versions_box{
        "Software Versions",
        fields,
        4
    };
    
    device_box.draw(&win, freespace);
    software_versions_box.draw(&win, freespace);

    auto* input_field = new Fl_Multiline_Input(BOX_START_X, freespace.get(), WINDOW_WIDTH - RIGHT_PANEL_MARGIN, 380);

    // Add the boxes to the container in order to read their data contents.
    container->boxes = {&device_box, &software_versions_box};

    container->in_field = input_field;

    int button_width = 100;
    Fl_Button* save =
    new Fl_Button((WINDOW_WIDTH - RIGHT_PANEL_MARGIN) + button_width / 2, 50, button_width, 30, "Save");

    save->callback(save_to_file, container);


    win.end();
    win.show();
    return Fl::run();
}

void save_to_file(Fl_Widget*, void* data) {
    auto* ctx = static_cast<Box_Data_Container*>(data);

    if (!ctx) return;
    if (!ctx->in_field) return;

    std::ofstream out("log.md");

    out << "# Device Log\n\n";

    for (const auto& box : ctx->boxes) {
        out << "## " << box->name << "\n";
        for (const auto& f : box->fields) {
            out << "- " << f.label << ": " << f.value << "\n";
        }
        out << "\n";
    }

    out << "## Notes\n";
    out << ctx->in_field->value() << "\n";

    std::cout << "[DEBUG] Saved log.md\n";
}


void draw_box(Fl_Window* win, Box& box, Freespace& freespace){
    int cols = box.cols;
    
    int box_start_x = BOX_START_X;
    int box_start_y = freespace.freespace;
    int col_width = ((WINDOW_WIDTH - (RIGHT_PANEL_MARGIN * (cols + 1))) - box_start_x) / cols;
    int y = MARGIN;

    // Cieling division. New row drawn when fields.size > cols.
    int rows = (box.fields.size() + (cols - 1)) / cols; 
    int group_box_height = rows * (TEXT_BOX_HEIGHT + MARGIN) + MARGIN;

    //win->begin();
    Fl_Group* group = new Fl_Group(box_start_x, box_start_y, WINDOW_WIDTH - RIGHT_PANEL_MARGIN, group_box_height, box.name.c_str());

    group->box(FL_UP_BOX); // visualize bounds
    group->align(FL_ALIGN_TOP_LEFT);
    group->labelsize(20);
    group->labelfont(5);
    group->labelcolor(FL_BLACK);


    // Come back to this to edit which input structure the data is saved to.
    for (size_t i = 0; i < box.fields.size(); ++i) {
        Field* field_ptr = &box.fields[i];

        int col = i % cols;
        int row = i / cols;

        int x = box_start_x + MARGIN  + col * (col_width + MARGIN);
        y = box_start_y + MARGIN + row * (TEXT_BOX_HEIGHT + MARGIN);

        Fl_Input* input = new Fl_Input(x, y, col_width, TEXT_BOX_HEIGHT, box.fields[i].label.c_str());
        input->align(FL_ALIGN_TOP_LEFT);  // label above box
        input->value(field_ptr->value.c_str());

        input->callback([](Fl_Widget* w, void* data){
            Field* f = static_cast<Field*>(data);
            f->value = static_cast<Fl_Input*>(w)->value();

        }, field_ptr);

    }

    freespace.add(group_box_height);
    group->end();  // IMPORTANT!
    //win->end();
}








/* void button_box(Fl_Window* win, int start_y){
    
    win->begin();
    // --- Section 1: Device Info ---

    // POS X, POS Y, WIDTH, HEIGHT
    Fl_Group* utility_buttons = new Fl_Group(BOX_START_X, start_y + ENTITY_SPACING,WINDOW_WIDTH -  MARGIN,(TEXT_BOX_HEIGHT + MARGIN) * 2, "Utiliy");
    utility_buttons->box(FL_UP_BOX); // visualize bounds
    utility_buttons->align(FL_ALIGN_TOP_LEFT);
    utility_buttons->labelsize(20);
    utility_buttons->labelfont(5);
    utility_buttons->labelcolor(FL_BLACK);


    Fl_Button btn(WINDOW_HEIGHT - MARGIN, MARGIN, 100, 40, "Save");

    btn.callback(save_cb);

    utility_buttons->end();  // IMPORTANT!
    win->end();
} */

