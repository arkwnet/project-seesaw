#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <string>

const std::string VERSION = "1.0.0";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Project Seesaw");
  window->color(FL_WHITE);
  Fl_Box *box_software_details = new Fl_Box(20, 10, 200, 40, "Software Details");
  box_software_details->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_details->labelsize(20);
  Fl_Box *box_software_version_left = new Fl_Box(20, 40, 200, 40, "Software Version:");
  box_software_version_left->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_version_left->labelsize(14);
  Fl_Box *box_software_version_right = new Fl_Box(220, 40, 200, 40, VERSION.c_str());
  box_software_version_right->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_version_right->labelsize(14);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}
