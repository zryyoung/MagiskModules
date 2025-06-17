#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <sstream>

using namespace std;

bool deleteFile(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            DIR* dir = opendir(path.c_str());
            if (!dir) return false;
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                string fullPath = path + "/" + entry->d_name;
                deleteFile(fullPath);
            }
            closedir(dir);
            return rmdir(path.c_str()) == 0;
        } else {
            return unlink(path.c_str()) == 0;
        }
    }
    return false;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

string stripComment(const string& line) {
    size_t hash = line.find('#');
    return trim(line.substr(0, hash));
}

bool parseConfig(const string& configPath, vector<string>& paths, int& interval, bool& enableLog, bool& loop) {
    ifstream file(configPath);
    if (!file.is_open()) return false;

    string line;
    string section;
    while (getline(file, line)) {
        line = stripComment(line);
        if (line.empty()) continue;

        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.length() - 2);
            continue;
        }

        if (section == "settings") {
            size_t eq = line.find('=');
            if (eq == string::npos) continue;
            string key = trim(line.substr(0, eq));
            string value = trim(line.substr(eq + 1));

            if (key == "interval") interval = stoi(value);
            else if (key == "log") {
                string valLower = value;
                for (char& c : valLower) c = tolower(c);
                enableLog = (valLower == "true" || valLower == "1");
            } else if (key == "loop") {
                string valLower = value;
                for (char& c : valLower) c = tolower(c);
                loop = (valLower == "true" || valLower == "1");
            }
        } else if (section == "paths") {
            string path = trim(line);
            if (!path.empty()) paths.push_back(path);
        }
    }
    return true;
}

void runOnce(const vector<string>& paths, bool log) {
    for (const auto& path : paths) {
        if (deleteFile(path)) {
            if (log) cout << "[+] Deleted: " << path << endl;
        } else {
            if (log) cout << "[-] Failed to delete: " << path << endl;
        }
    }
}

int main() {
    const string configPath = "./config";
    vector<string> paths;
    int interval = 60;
    bool log = true;
    bool loop = true;

    if (!parseConfig(configPath, paths, interval, log, loop)) {
        cerr << "[!] Failed to parse config" << endl;
        return 1;
    }

    if (log) {
        cout << "[*] Loaded " << paths.size() << " entries" 
             << ", interval: " << interval << "s"
             << ", loop: " << (loop ? "true" : "false") << endl;
    }

    if (loop) {
        while (true) {
            runOnce(paths, log);
            sleep(interval);
        }
    } else {
        runOnce(paths, log);
    }

    return 0;
}