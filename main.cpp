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
  temp = search(pipe, "Chassis Information", "Type");
  Fl_Box *box_hardware_chassis_left = box(20, 160, 200, 20, 14, "Chassis type:");
  Fl_Box *box_hardware_chassis_right = box(220, 160, 200, 20, 14, temp.c_str());
  temp = search(pipe, "System Information", "SKU Number");
  Fl_Box *box_hardware_model_left = box(20, 180, 200, 20, 14, "Model:");
  Fl_Box *box_hardware_model_right = box(220, 180, 200, 20, 14, temp.c_str());
  temp = search(pipe, "System Information", "Serial Number");
  Fl_Box *box_hardware_serial_left = box(20, 200, 200, 20, 14, "Serial:");
  Fl_Box *box_hardware_serial_right = box(220, 200, 200, 20, 14, temp.c_str());
  temp = search(pipe, "System Information", "UUID");
  Fl_Box *box_hardware_uuid_left = box(20, 220, 200, 20, 14, "UUID:");
  Fl_Box *box_hardware_uuid_right = box(220, 220, 200, 20, 14, temp.c_str());
  temp = search(pipe, "Chassis Information", "Asset Tag");
  Fl_Box *box_hardware_tag_left = box(20, 240, 200, 20, 14, "Asset tag:");
  Fl_Box *box_hardware_tag_right = box(220, 240, 200, 20, 14, temp.c_str());
  temp = search(pipe, "Processor Information", "Version");
  Fl_Box *box_hardware_processor_left = box(20, 260, 200, 20, 14, "Processor:");
  Fl_Box *box_hardware_processor_right = box(220, 260, 200, 20, 14, temp.c_str());
  temp = search(pipe, "Memory Device", "Size");
  Fl_Box *box_hardware_memory_left = box(20, 280, 200, 20, 14, "Total memory:");
  Fl_Box *box_hardware_memory_right = box(220, 280, 200, 20, 14, temp.c_str());

  temp = search(pipe, "Base Board Information", "Manufacturer") + " / " + search(pipe, "Base Board Information", "Product Name");
  Fl_Box *box_hardware_motherboard_left = box(20, 300, 200, 20, 14, "Motherboard:");
  Fl_Box *box_hardware_motherboard_right = box(220, 300, 200, 20, 14, temp.c_str());

  temp = search(pipe, "BIOS Information", "Vendor") + ", Version: " + search(pipe, "BIOS Information", "Version") + ", Date: " + search(pipe, "BIOS Information", "Release Date") + ", ROM size: " + search(pipe, "BIOS Information", "ROM Size");
  Fl_Box *box_hardware_bios_left = box(20, 320, 200, 20, 14, "BIOS information:");
  Fl_Box *box_hardware_bios_right = box(220, 320, 200, 20, 14, temp.c_str());

  pclose(pipe);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}
