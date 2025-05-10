#include <iostream>
#include <string>
#include <cstdlib>    // For system, EXIT_SUCCESS, EXIT_FAILURE
#include <cctype>     // For std::toupper
#include <filesystem> // For std::filesystem (C++17)
#include <vector>     // For fs::remove_all alternative for build/*
#include <algorithm>  // For std::remove for path manipulation (not strictly needed here now)

// Configuration
const std::string BUILD_DIR = "build";
const int NUM_JOBS = 8; // For parallel builds

namespace fs = std::filesystem;

// Helper function to execute a system command and check its result
// Exits the program if the command fails
void execute_command(const std::string& command_str, const std::string& description) {
    std::cout << "Executing: " << command_str << " (" << description << ")" << std::endl;
    int result = system(command_str.c_str());
    if (result != 0) {
        // system() return value is complex, but non-zero usually indicates an error from the command.
        std::cerr << "Error: Command failed with exit code " << result << " during: " << description << "." << std::endl;
        std::cerr << "Command: " << command_str << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Successfully executed: " << description << std::endl;
}

void clean_build_directory() {
    std::cout << "Preparing to clean " << BUILD_DIR << "/* ..." << std::endl;
    if (fs::exists(BUILD_DIR)) {
<<<<<<< HEAD
        // More robust way than "rm -rf build/*" which might fail or have unintended consequences
        // This removes all contents of BUILD_DIR but not BUILD_DIR itself.
        // If BUILD_DIR itself should be removed and recreated:
        // fs::remove_all(BUILD_DIR);
        // fs::create_directory(BUILD_DIR);
        // For now, let's replicate "rm -rf build/*" meaning:
        size_t Sremoved_count = 0;
        for (const auto& entry : fs::directory_iterator(BUILD_DIR)) {
            fs::remove_all(entry.path()); // Safely remove files and subdirectories within BUILD_DIR
            Sremoved_count++;
        }
        if (Sremoved_count > 0) {
            fmt::print("Cleaned contents of {} directory.\n", BUILD_DIR);
        } else {
            fmt::print("{} directory was empty or only contained dotfiles not matched by '/*'.\n", BUILD_DIR);
=======
        size_t removed_count = 0;
        try {
            for (const auto& entry : fs::directory_iterator(BUILD_DIR)) {
                fs::remove_all(entry.path()); // Safely remove files and subdirectories within BUILD_DIR
                removed_count++;
            }
            if (removed_count > 0) {
                std::cout << "Cleaned contents of " << BUILD_DIR << " directory." << std::endl;
            } else {
                std::cout << BUILD_DIR << " directory was empty or already clean." << std::endl;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error cleaning " << BUILD_DIR << " directory: " << e.what() << std::endl;
            // Decide if this is fatal. For a clean step, maybe just warn.
            // For now, let's continue, as CMake can often recover.
            // If critical, add: std::exit(EXIT_FAILURE);
>>>>>>> 09d297c08247a2735b63f5c04a8df15277076c5c
        }
    } else {
        std::cout << BUILD_DIR << " directory does not exist. Creating it." << std::endl;
        try {
            fs::create_directory(BUILD_DIR);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error: Failed to create directory " << BUILD_DIR << ": " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

std::string choose_generator_and_configure() {
    char mode_char_input;
    std::string generator_name;
    std::string cmake_generator_arg;
    std::string build_tool_command; // Not directly used anymore with `cmake --build`

    while (true) {
        std::cout << "Please choose builder: [N]inja or [M]ake (default): " << std::flush;
        if (!(std::cin >> mode_char_input)) {
            std::cerr << "Error reading input. Exiting." << std::endl;
            if (std::cin.eof()) { // Handle EOF case (e.g., input redirected from /dev/null)
                 std::cerr << "EOF reached on input." << std::endl;
            }
            std::cin.clear(); // Clear error flags
            // Consume the rest of the problematic line
            std::string dummy;
            std::getline(std::cin, dummy); 
            // Potentially exit or offer a default if input fails repeatedly
            // For now, exiting as per original intent on failure
            std::exit(EXIT_FAILURE); 
        }
        // Consume rest of the line to handle cases like "N abc"
        std::string dummy;
        std::getline(std::cin, dummy);

        mode_char_input = static_cast<char>(std::toupper(mode_char_input));

        if (mode_char_input == 'N') {
            generator_name = "Ninja";
            cmake_generator_arg = "-G Ninja";
            build_tool_command = "ninja";
            break;
        } else if (mode_char_input == 'M') {
            generator_name = "Make";
            cmake_generator_arg = ""; // No explicit -G for default Makefiles
            build_tool_command = "make";
            break;
        } else {
            std::cout << "Invalid option '" << mode_char_input << "'. Please choose N or M." << std::endl;
        }
    }

    std::cout << "Configuring project with " << generator_name << "..." << std::endl;

    std::string cmake_command_str = "cmake -S . -B " + BUILD_DIR;
    if (!cmake_generator_arg.empty()) {
        cmake_command_str += " " + cmake_generator_arg;
    }
    execute_command(cmake_command_str, "CMake configuration");
    
    return build_tool_command; // Maintained for conceptual clarity but not used by `build_project`
}

void build_project(const std::string& /*build_tool_cmd_unused*/) {
    std::cout << "Building project (using cmake --build)..." << std::endl;

    std::string build_command_str = "cmake --build " + BUILD_DIR + " --parallel " + std::to_string(NUM_JOBS);
    execute_command(build_command_str, "Compilation");
}

int main() {
    clean_build_directory();
    std::string build_tool = choose_generator_and_configure(); 
    // build_tool is not directly used by build_project anymore but function signature maintained
    build_project(build_tool); 

    std::cout << "Build process completed successfully." << std::endl;
    return EXIT_SUCCESS;
}
