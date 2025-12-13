#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const std::string PRODUCT_NAME = "Project Seesaw";
const std::string PRODUCT_VERSION = "1.0.0";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

std::string timestamp() {
  using namespace std::chrono;
  auto now = system_clock::now();
  std::time_t t = system_clock::to_time_t(now);
  std::tm lt = *std::localtime(&t);
  auto subseconds = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  std::ostringstream oss;
  oss << std::put_time(&lt, "%Y-%m-%dT%H:%M:%S");
  oss << "." << std::setw(3) << std::setfill('0') << subseconds.count();
  char buf[8];
  std::strftime(buf, sizeof(buf), "%z", &lt);
  std::string tz(buf);
  tz.insert(3, ":");
  oss << tz;
  return oss.str();
}

Fl_Box* box(int x, int y, int w, int h, int label_size, const char* text) {
  Fl_Box* box = new Fl_Box(x, y, w, h);
  box->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box->labelsize(label_size);
  if (text) {
    box->copy_label(text);
  }
  return box;
}

std::string search(FILE* pipe, const std::string& section, const std::string& key) {
  if (!pipe) {
    return "";
  }
  std::string line;
  char buffer[256];
  bool in_target_handle = false;
  while (fgets(buffer, sizeof(buffer), pipe)) {
    line = buffer;
    if (!line.empty() && line[0] != '\t' && line.find(section) != std::string::npos) {
      in_target_handle = true;
      continue;
    }
    if (!line.empty() && line[0] != '\t' && in_target_handle) {
      break;
    }
    if (in_target_handle) {
      std::string label = "\t" + key + ":";
      if (line.find(label) == 0) {
        std::string value = line.substr(label.size());
        size_t start = value.find_first_not_of(" \t");
        size_t end = value.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
          value = value.substr(start, end - start + 1);
        }
        return value;
      }
    }
  }
  return "";
}

int main(int argc, char **argv) {
  std::string temp;
  Fl_Window *window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Project Seesaw");
  window->color(FL_WHITE);

  Fl_Box *box_software_details = box(20, 10, 200, 40, 20, "Software Details");

  std::string now = timestamp();
  Fl_Box *box_software_date_left = box(20, 50, 200, 20, 14, "Date/Time:");
  Fl_Box *box_software_date_right = box(220, 50, 200, 20, 14, now.c_str());

  temp = PRODUCT_NAME + ", " + PRODUCT_VERSION;
  Fl_Box *box_software_version_left = box(20, 70, 200, 20, 14, "Software Version:");
  Fl_Box *box_software_version_right = box(220, 70, 200, 20, 14, temp.c_str());

  Fl_Box *box_hardware_details = box(20, 100, 200, 40, 20, "Hardware Details");

  FILE* pipe = popen("sudo dmidecode", "r");
  temp = search(pipe, "System Information", "Manufacturer");
  Fl_Box *box_hardware_manufacturer_left = box(20, 140, 200, 20, 14, "Manufacturer:");
  Fl_Box *box_hardware_manufacturer_right = box(220, 140, 200, 20, 14, temp.c_str());

  pclose(pipe);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}
