#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Box.H>

#include <fstream>
#include <vector>
#include <FL/fl_draw.H>
#include <iostream>
#include <sstream>


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
void popup_cb(Fl_Widget*, void*);
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
        {"Date:"}, {"Device ID:"}, {"Operatodasdasdasdasdasdasdasdasdasdasdr:"}, {"Comment:"}, {"TEST:"}, {"TEST:"},
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
    new Fl_Button((WINDOW_WIDTH - RIGHT_PANEL_MARGIN) + button_width / 2, 50, button_width, 50, "Save");
    
    save->callback(save_to_file, container);

    Fl_Button* search = new Fl_Button((WINDOW_WIDTH - RIGHT_PANEL_MARGIN) + button_width / 2, 100, button_width, 50, "Search");
    
    search->callback(popup_cb);



    win.end();
    win.show();
    return Fl::run();
}
// Buttons needs to accept a Fl_Widget pointer and a nullpointer (data).
void popup_cb(Fl_Widget*, void*){

    int popup_length = 800;

    Fl_Window* popup = new Fl_Window(WINDOW_WIDTH/2, 800, "Search Log");

    int x = MARGIN;
    int y = MARGIN;

    // Get the windows width, subtract the margin times 2.
    int w = popup->w() - 2 * MARGIN;

    Fl_Box* label = new Fl_Box(x, y, w, 20, "Search for Device:");
    
    int search_box_height = 40;

    Fl_Input* search = new Fl_Input(x, y + MARGIN, w, search_box_height);


        // Text output
    Fl_Text_Display* display = new Fl_Text_Display(x, y + search_box_height + MARGIN, w, popup->h() - 100); // Hardcoded for now
    Fl_Text_Buffer* buffer = new Fl_Text_Buffer();
    display->buffer(buffer);

        // Search button
    Fl_Button* go = new Fl_Button(400, 20, 80, 25, "Go");


    /* function */
    /*
    A pointer to the function (the lambda)

    A raw void* pointer (your new std::pair<...>)

    Think of it as:

    Button "go":
    callback_fn = <that lambda>
    user_data  = 0x7fffa1234   (heap address)
    

    Has no captures ([])

    Therefore decays into a plain C function pointer

    Matches FLTK’s required signature exactly
    
    Captured Lambdas are objects, not function pointers..

    RUNTIME:

    - User clicks Go → FLTK internally does:
    - callback_fn(go_button, user_data);

    Which becomes:

    - lambda(go_button, 0x7fffa1234);

    Now we are inside the lambda.

    */ 
    go->callback([](Fl_Widget*, void* data){
        /* The data pointer which we pass later, and the data will have the Type(input, buffer) */


        /*
        data points to a heap-allocated std::pair

        FLTK has no idea what’s inside

        You must cast it back yourself

        This cast says:

        “Trust me — this void* actually points to a pair<Fl_Input*, Fl_Text_Buffer*>.”

        
        What ctx represents
        ctx->first   → Fl_Input*       (search field)
        ctx->second  → Fl_Text_Buffer* (output buffer)

        This pair is your shared state.

        FLTK does not manage this.
        */
        auto* ctx = static_cast<std::pair<Fl_Input*, Fl_Text_Buffer*>*>(data);

        /* This pulls current text from the input field — not cached, not stored earlier. */
        std::string query = ctx->first->value();

        // Opens file
        std::ifstream file("log.md");
    
        std::ostringstream out;
        std::string line;

        while (std::getline(file, line)) {
            out << line << "\n";
        }
        ctx->second->text(out.str().c_str());

    }, new std::pair<Fl_Input*, Fl_Text_Buffer*>(search, buffer));

    popup->end();
    popup->show();
}

void save_to_file(Fl_Widget*, void* data) {
    auto* ctx = static_cast<Box_Data_Container*>(data);

    if (!ctx) return;
    if (!ctx->in_field) return;

    std::string name;


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

