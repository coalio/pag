#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include <numeric>
#include <map>

void print_help(const char* executable_name) {
  std::cout << "Usage: " << executable_name << " [options] [-p | <file>]\n";
  std::cout << "Options:\n";
  std::cout << "  -h, --help\t\t\t\tDisplay this help message\n";
  std::cout << "  -m, --maxlines <lines>\t\tSet how many lines per page will be displayed\n";
  std::cout << "  -s, --startline <line>\t\tSet the line to start from\n";
  std::cout << "  -e, --endline <line>\t\t\tSet the line to end at\n";
  std::cout << "  -l, --maxlength <length>\t\tSet the max length of the line" << std::endl;
}

void draw_input_bar() {
  // Draws an input bar at the bottom of the screen
  // Add a newline then print the prompt
  std::string prompt = "Press enter to display next page or ':q' to exit:";

  std::cout << std::endl;
  std::cout << "\033[0;37;40m"
            << prompt;

  // Reset the color to default
  std::cout << "\033[0m :";
  // Flush the buffer to make sure the input bar is drawn
  // before the user can start typing
  std::cout.flush(); 
}

void erase_input_bar() {
  // Erase the input bar
  // Move the cursor to the bottom of the screen
  // and clear the line
  std::cout << "\033[A\033[K";
  // Flush the buffer to make sure the input bar is erased
  std::cout.flush();
}

int main(const int argc, const char* argv[]) {
  std::map<std::string, std::string> options;
  std::string filename;

  // Parse command line options
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg.find("-") == 0) {
      if (arg.find("--") == 0) {
        arg = arg.substr(2);
      } else {
        arg = arg.substr(1);
      }

      if (arg == "help" || arg == "h") {
        print_help(argv[0]);
        return 0;
      } else if (arg == "maxlines" || arg == "m") {
        if (i + 1 < argc) {
          options["maxlines"] = argv[++i];
        } else {
          std::cerr << "Error: missing argument for " << arg << std::endl;
          return 1;
        }
      } else if (arg == "startline" || arg == "s") {
        if (i + 1 < argc) {
          options["startline"] = argv[++i];
        } else {
          std::cerr << "Error: missing argument for " << arg << std::endl;
          return 1;
        }
      } else if (arg == "endline" || arg == "e") {
        if (i + 1 < argc) {
          options["endline"] = argv[++i];
        } else {
          std::cerr << "Error: missing argument for " << arg << std::endl;
          return 1;
        }
      } else if (arg == "maxlength" || arg == "l") {
        if (i + 1 < argc) {
          options["maxlength"] = argv[++i];
        } else {
          std::cerr << "Error: missing argument for " << arg << std::endl;
          return 1;
        }
      } else {
        std::cerr << "Error: unknown option " << arg << std::endl;
        return 1;
      }
    } else {
      if (filename.empty()) {
        filename = arg;
      } else {
        std::cerr << "Error: too many arguments" << std::endl;
        return 1;
      }
    }
  }

  // If file is not specified, print usage and exit
  if (filename.empty() && !options.count("pipe")) {
    std::cerr << "Error: missing file argument\n\n";
    print_help(argv[0]);
    return 1;
  }

  std::istream* input;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: could not open file " << filename << std::endl;
    return 1;
  }

  // Convert file contents to string and save to input
  std::string contents = std::string((std::istreambuf_iterator<char>(file)),
                                      std::istreambuf_iterator<char>());

  file.close();
  input = new std::istringstream(contents);

  // Parse options
  std::string::size_type max_lines = 10;
  std::string::size_type start_line = 0;
  std::string::size_type end_line = -1;
  std::string::size_type max_length = -1;

  // Convert options to ints and set their corresponding variables
  if (options.count("maxlines")) {
    try {
      max_lines = std::stoi(options["maxlines"]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Error: invalid argument for maxlines: " << options["maxlines"] << std::endl;
      return 1;
    }
  }
  
  if (options.count("maxlength")) {
    try {
      max_length = std::stoi(options["maxlength"]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Error: invalid argument for maxlength: " << options["maxlength"] << std::endl;
      return 1;
    }
  }
  
  if (options.count("startline")) {
    try {
      start_line = std::stoi(options["startline"]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Error: invalid argument for startline: " << options["startline"] << std::endl;
      return 1;
    }
  }
  
  if (options.count("endline")) {
    try {
      end_line = std::stoi(options["endline"]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Error: invalid argument for endline: " << options["endline"] << std::endl;
      return 1;
    }
  }

  // Read chunks
  std::vector<std::string> pages;
  std::vector<std::string> lines;
  std::string line;
  // Skip the first start_line lines (1 being the first line)
  // (yeah the only way to ignore the first start_line lines is to read them and use them for nothing)
  for (long unsigned int i = 0; i < start_line; i++) {
    std::getline(*input, line);
  }

  while (std::getline(*input, line)) {
    // If line is too long, split it and add "..." at the end
    if ((int) max_length != -1 && line.length() > max_length) {
      line = line.substr(0, max_length) + "...";
    }

    lines.push_back(line + "\n");
    if (lines.size() == max_lines) {
      // Remove newline from last line
      lines.back() = lines.back().substr(0, lines.back().length() - 1);
      pages.push_back(
        std::accumulate(lines.begin(), lines.end(), std::string())
      );
      lines.clear();
    }

    if ((int) end_line != -1 && lines.size() == end_line) {
      break;
    }
  }

  // Add last page
  if (!lines.empty()) {
    pages.push_back(
      std::accumulate(lines.begin(), lines.end(), std::string())
    );
    lines.clear();
  }

  // Allow the user to print pages one by one
  // Everytime enter is pressed, the next page is printed
  for (long unsigned int i = 0; i < pages.size(); i++) {
    std::cout << pages[i];
    std::string input;
    draw_input_bar();

    input = std::cin.get();
    
    if (!input.empty())
    {
      if (input == "q") {
        break;
      }
    }

    erase_input_bar();
  }

  return 0;
}