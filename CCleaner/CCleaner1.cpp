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

// 删除文件或目录，目录递归删除
bool deleteFile(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;

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

// 递归删除指定目录下的空文件和空文件夹，返回是否对目录做过修改（删除）
bool cleanEmptyFilesAndFolders(const string& path, bool log) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;

    if (S_ISDIR(st.st_mode)) {
        DIR* dir = opendir(path.c_str());
        if (!dir) return false;

        bool removedAny = false;
        struct dirent* entry;
        vector<string> children;

        while ((entry = readdir(dir)) != nullptr) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            children.push_back(entry->d_name);
        }
        closedir(dir);

        // 先递归处理子目录
        for (const auto& child : children) {
            string fullPath = path + "/" + child;
            if (cleanEmptyFilesAndFolders(fullPath, log)) {
                removedAny = true;
            }
        }

        // 目录内容更新后再判断是否空目录
        dir = opendir(path.c_str());
        if (!dir) return removedAny;

        bool isEmpty = true;
        while ((entry = readdir(dir)) != nullptr) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            isEmpty = false;
            break;
        }
        closedir(dir);

        if (isEmpty) {
            if (rmdir(path.c_str()) == 0) {
                if (log) cout << "[+] Deleted empty folder: " << path << endl;
                return true;
            } else {
                if (log) cout << "[-] Failed to delete empty folder: " << path << endl;
            }
        }
        return removedAny;
    } else {
        // 是文件，判断是否大小为0，若是则删除
        if (st.st_size == 0) {
            if (unlink(path.c_str()) == 0) {
                if (log) cout << "[+] Deleted empty file: " << path << endl;
                return true;
            } else {
                if (log) cout << "[-] Failed to delete empty file: " << path << endl;
            }
        }
    }
    return false;
}

// 删除目录下所有内容，但保留目录本身（递归）
bool deleteAllContents(const string& path, bool log) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;

    if (!S_ISDIR(st.st_mode)) return false;

    DIR* dir = opendir(path.c_str());
    if (!dir) return false;

    struct dirent* entry;
    bool success = true;

    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string fullPath = path + "/" + entry->d_name;
        if (!deleteFile(fullPath)) {
            success = false;
            if (log) cout << "[-] Failed to delete: " << fullPath << endl;
        } else {
            if (log) cout << "[+] Deleted: " << fullPath << endl;
        }
    }

    closedir(dir);
    return success;
}

// 清理 /sdcard/Android/data/*/cache 目录
void cleanAppCaches(const string& androidDataPath, bool log) {
    struct stat st;
    if (stat(androidDataPath.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
        if (log) cout << "[-] Android data path not found: " << androidDataPath << endl;
        return;
    }

    DIR* dir = opendir(androidDataPath.c_str());
    if (!dir) {
        if (log) cout << "[-] Failed to open: " << androidDataPath << endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string appDir = androidDataPath + "/" + entry->d_name;
        struct stat appSt;
        if (stat(appDir.c_str(), &appSt) == 0 && S_ISDIR(appSt.st_mode)) {
            string cacheDir = appDir + "/cache";
            struct stat cacheSt;
            if (stat(cacheDir.c_str(), &cacheSt) == 0 && S_ISDIR(cacheSt.st_mode)) {
                if (log) cout << "[*] Cleaning cache: " << cacheDir << endl;
                deleteAllContents(cacheDir, log);
            }
        }
    }
    closedir(dir);
}

// 读取配置
bool parseConfig(const string& configPath, bool& cache, bool& emptyFilesAndFolders, bool& log) {
    ifstream file(configPath);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        // 去除注释和空白
        size_t hashPos = line.find('#');
        if (hashPos != string::npos) line = line.substr(0, hashPos);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        size_t eqPos = line.find('=');
        if (eqPos == string::npos) continue;

        string key = line.substr(0, eqPos);
        string val = line.substr(eqPos + 1);

        // 去除key和val的空白
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        val.erase(0, val.find_first_not_of(" \t\r\n"));
        val.erase(val.find_last_not_of(" \t\r\n") + 1);

        for (auto & c : val) c = tolower(c);

        if (key == "cache") cache = (val == "true" || val == "1");
        else if (key == "emptyfilesandfolders") emptyFilesAndFolders = (val == "true" || val == "1");
        else if (key == "log") log = (val == "true" || val == "1");
    }

    return true;
}

int main() {
    bool cache = false;
    bool emptyFilesAndFolders = false;
    bool log = true;

    const string configPath = "./config";

    if (!parseConfig(configPath, cache, emptyFilesAndFolders, log)) {
        cerr << "[!] Failed to parse config file." << endl;
        return 1;
    }

    if (log) {
        cout << "[*] Config - cache: " << (cache ? "true" : "false")
             << ", EmptyFilesAndFolders: " << (emptyFilesAndFolders ? "true" : "false") << endl;
    }

    if (emptyFilesAndFolders) {
        if (log) cout << "[*] Start cleaning empty files and folders under /sdcard ..." << endl;
        cleanEmptyFilesAndFolders("/sdcard", log);
    }

    if (cache) {
        if (log) cout << "[*] Start cleaning app caches under /sdcard/Android/data ..." << endl;
        cleanAppCaches("/sdcard/Android/data", log);
    }

    if (log) cout << "[*] Clean completed." << endl;

    return 0;
}