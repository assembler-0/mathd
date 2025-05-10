#include <iostream>
#include <string>
#include <cstdlib>    // For system, EXIT_SUCCESS, EXIT_FAILURE
#include <cctype>     // For std::toupper
#include <filesystem> // For std::filesystem (C++17)
#include <fmt/core.h> // For fmt::print

// Configuration
const std::string BUILD_DIR = "build";
const int NUM_JOBS = 8; // For parallel builds

namespace fs = std::filesystem;

// Helper function to execute a system command and check its result
// Exits the program if the command fails
void execute_command(const std::string& command_str, const std::string& description) {
    fmt::print("Executing: {} ({})\n", command_str, description);
    int result = system(command_str.c_str());
    if (result != 0) {
        // system() return value is complex, but non-zero usually indicates an error from the command.
        // For more detailed error checking, platform-specific code or a library like Boost.Process would be needed.
        fmt::print(stderr, "Error: Command failed with exit code {} during: {}.\nCommand: {}\n", result, description, command_str);
        std::exit(EXIT_FAILURE);
    }
    fmt::print("Successfully executed: {}\n", description);
}

void clean_build_directory() {
    fmt::print("Preparing to clean {}/* ...\n", BUILD_DIR);
    if (fs::exists(BUILD_DIR)) {
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
        }
    } else {
        fmt::print("{} directory does not exist. Creating it.\n", BUILD_DIR);
        try {
            fs::create_directory(BUILD_DIR);
        } catch (const fs::filesystem_error& e) {
            fmt::print(stderr, "Error: Failed to create directory {}: {}\n", BUILD_DIR, e.what());
            std::exit(EXIT_FAILURE);
        }
    }
     // Alternative using CMake's capabilities for cleaning (might be safer or more cross-platform for specific files):
    // if (fs::exists(BUILD_DIR)) {
    //    execute_command(fmt::format("cmake -E rm -rf {}/*", BUILD_DIR), "Cleaning build directory content");
    // }
    // execute_command(fmt::format("cmake -E make_directory {}", BUILD_DIR), "Ensuring build directory exists");
}

std::string choose_generator_and_configure() {
    char mode_char;
    std::string generator_name;
    std::string cmake_generator_arg;
    std::string build_tool_command;

    while (true) {
        fmt::print("Please choose builder: [N]inja or [M]ake (default): ");
        // Make sure to consume the newline character from previous prints if any
        // or if this loop runs multiple times.
        if (!(std::cin >> mode_char)) {
            fmt::print(stderr, "Error reading input. Exiting.\n");
            std::exit(EXIT_FAILURE);
        }
        // Consume rest of the line to handle cases like "N abc"
        std::string dummy;
        std::getline(std::cin, dummy); 


        mode_char = static_cast<char>(std::toupper(mode_char));

        if (mode_char == 'N') {
            generator_name = "Ninja";
            cmake_generator_arg = "-G Ninja";
            build_tool_command = "ninja"; // Correct tool for building
            break;
        } else if (mode_char == 'M') {
            generator_name = "Make"; // Default CMake generator often is Makefiles
            cmake_generator_arg = ""; // No explicit -G needed for default
            build_tool_command = "make"; // Correct tool
            break;
        } else {
            fmt::print("Invalid option '{}'. Please choose N or M.\n", mode_char);
        }
    }

    fmt::print("Configuring project with {}...\n", generator_name);
    // Use -S for source directory and -B for build directory, avoids `cd`
    std::string cmake_command = fmt::format("cmake -S . -B {} {}", BUILD_DIR, cmake_generator_arg);
    execute_command(cmake_command, "CMake configuration");
    
    return build_tool_command; // Return the command for the build step (e.g. "make" or "ninja")
                               // Although, cmake --build is even better
}

void build_project(const std::string& build_tool_cmd_unused) {
    // build_tool_cmd is now unused because `cmake --build` is more robust
    fmt::print("Building project (using cmake --build)...\n");

    // `cmake --build` is the recommended way to invoke the underlying build system (Make, Ninja, MSBuild, etc.)
    // It automatically uses the correct build tool based on the generated project files.
    // The `--parallel` flag is often supported and more portable than `-jX`.
    std::string build_command = fmt::format("cmake --build {} --parallel {}", BUILD_DIR, NUM_JOBS);
    execute_command(build_command, "Compilation");
}

int main() {
    clean_build_directory();
    std::string build_tool = choose_generator_and_configure();
    // The `build_tool` string is kept in case you want to manually call make/ninja,
    // but `build_project` now uses `cmake --build`.
    build_project(build_tool); 

    fmt::print("Build process completed successfully.\n");
    return EXIT_SUCCESS;
}
