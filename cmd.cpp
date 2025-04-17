#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <direct.h>  // For _chdir, _getcwd, _fullpath
#include <stdio.h>
#include <vector>

std::string get_last_dir() {
    char buffer[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, buffer)) {
        return std::string(buffer);
    }
    return ""; // Fallback in case of error
}

void save_dir(const std::string& dir) {
    std::ofstream file("log.txt");
    file << dir;
    file.close();
}

std::string exec_command(const std::string& cmd) {
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) {
        return "[-] Error executing command.";
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    return result;
}

// Resolves relative paths
std::string resolve_path(const std::string& path) {
    char resolved_path[MAX_PATH];
    if (_fullpath(resolved_path, path.c_str(), MAX_PATH) != NULL) {
        return std::string(resolved_path);
    }
    return "";  // Return empty if path resolution fails
}

int main() {
    std::string cwd = get_last_dir();

    // If a previous directory is found, load it from log.txt
    std::ifstream file("log.txt");
    if (file.is_open()) {
        std::getline(file, cwd);
        file.close();
    }

    while (true) {
        std::cout << cwd << "> ";
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd.empty()) continue;

        // Handle "cd .."
        if (cmd == "cd ..") {
            // Change directory to the parent directory
            char new_cwd[MAX_PATH];
            if (_getcwd(new_cwd, MAX_PATH)) {
                std::string new_path(new_cwd);
                size_t pos = new_path.find_last_of("\\/");
                if (pos != std::string::npos) {
                    new_path = new_path.substr(0, pos);  // Remove the last part to move to parent
                    if (_chdir(new_path.c_str()) == 0) {
                        cwd = new_path;
                        save_dir(cwd);
                    } else {
                        std::cout << "[-] Error changing to parent directory.\n";
                    }
                }
            }
            continue;
        }

        // Handle "cd <directory>"
        if (cmd.substr(0, 3) == "cd ") {
            std::string path = cmd.substr(3);

            // If the path is empty, just show the current directory
            if (path.empty()) {
                std::cout << cwd << "\n";
                continue;
            }

            // If the path is relative, resolve it to an absolute path
            std::string full_path = resolve_path(path);
            if (_chdir(full_path.c_str()) == 0) {
                cwd = full_path;
                save_dir(cwd);
            } else {
                std::cout << "[-] Directory not found: " << path << "\n";
            }
            continue;
        }

        // Run command and display output
        std::string output = exec_command(cmd);
        std::cout << output;
    }

    return 0;
}
