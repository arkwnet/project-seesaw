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

std::string search(FILE* pipe, const std::string& handle_key, const std::string& type_key, const std::string& field_key) {
  if (!pipe) {
    return "";
  }
  std::string line;
  char buffer[256];
  bool in_target_handle = false;
  while (fgets(buffer, sizeof(buffer), pipe)) {
      line = buffer;
      if (line.find(handle_key) != std::string::npos && line.find(type_key) != std::string::npos) {
        in_target_handle = true;
        continue;
      }
      if (in_target_handle && line.find("Handle ") != std::string::npos) {
        break;
      }
      if (in_target_handle && line.find(field_key) != std::string::npos) {
        std::string value = line.substr(line.find(":") + 1);
        while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) {
          value.erase(0, 1);
        }
        while (!value.empty() && (value.back() == '\n' || value.back() == '\r')) {
          value.pop_back();
        }
        return value;
      }
  }
  return "";
}

int main(int argc, char **argv) {
  std::string temp;
  Fl_Window *window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Project Seesaw");
  window->color(FL_WHITE);

  Fl_Box *box_software_details = new Fl_Box(20, 10, 200, 40, "Software Details");
  box_software_details->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_details->labelsize(20);

  std::string now = timestamp();
  Fl_Box *box_software_date_left = new Fl_Box(20, 50, 200, 20, "Date/Time:");
  box_software_date_left->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_date_left->labelsize(14);
  Fl_Box *box_software_date_right = new Fl_Box(220, 50, 200, 20, now.c_str());
  box_software_date_right->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_date_right->labelsize(14);

  temp = PRODUCT_NAME + ", " + PRODUCT_VERSION;
  Fl_Box *box_software_version_left = new Fl_Box(20, 70, 200, 20, "Software Version:");
  box_software_version_left->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_version_left->labelsize(14);
  Fl_Box *box_software_version_right = new Fl_Box(220, 70, 200, 20);
  box_software_version_right->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_software_version_right->labelsize(14);
  box_software_version_right->copy_label(temp.c_str());

  Fl_Box *box_hardware_details = new Fl_Box(20, 100, 200, 40, "Hardware Details");
  box_hardware_details->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_hardware_details->labelsize(20);

  FILE* pipe = popen("sudo dmidecode", "r");
  temp = search(pipe, "Handle 0x000C", "DMI type 1", "Manufacturer:");
  Fl_Box *box_hardware_manufacturer_left = new Fl_Box(20, 140, 200, 20, "Manufacturer:");
  box_hardware_manufacturer_left->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_hardware_manufacturer_left->labelsize(14);
  Fl_Box *box_hardware_manufacturer_right = new Fl_Box(220, 140, 200, 20);
  box_hardware_manufacturer_right->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box_hardware_manufacturer_right->labelsize(14);
  box_hardware_manufacturer_right->copy_label(temp.c_str());

  pclose(pipe);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}
