#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
using namespace std;

void listDir(string path) {
    DIR *dir = opendir(path.c_str());
    dirent *entry;
    cout << "\nDirectory: " << path << endl;

    while ((entry = readdir(dir)) != NULL) {
        cout << " - " << entry->d_name << endl;
    }
    closedir(dir);
}

void showHelp() {
    cout << "\nAvailable Commands:\n";
    cout << "--------------------------------------------------\n";
    cout << "ls       - List files in current directory\n";
    cout << "cd       - Change directory (example: cd foldername)\n";
    cout << "new      - Create new file (example: new file.txt)\n";
    cout << "del      - Delete a file (example: del file.txt)\n";
    cout << "ren      - Rename file (example: ren old.txt new.txt)\n";
    cout << "cp       - Copy file (example: cp file1.txt file2.txt)\n";
    cout << "search   - Search for a file by name (recursive)\n";
    cout << "chmod    - Change file permissions (example: chmod 755 file)\n";
    cout << "clear    - Clear the terminal screen\n";
    cout << "exit     - Quit file explorer\n";
    cout << "--------------------------------------------------\n";
}

void searchFile(string base, string target) {
    DIR *dir = opendir(base.c_str());
    if (!dir) return;

    dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        string fullPath = base + "/" + name;

        if (name == target)
            cout << "Found: " << fullPath << endl;

        struct stat info;
        stat(fullPath.c_str(), &info);

        if (S_ISDIR(info.st_mode) && name != "." && name != "..")
            searchFile(fullPath, target);
    }
    closedir(dir);
}

int main() {
    string currentPath = ".";
    string cmd, arg1, arg2;
    cout<<"Write 'help' to see all the commands available"<<endl;
    while (true) {
        cout << "\n> ";
        cin >> cmd;

        if (cmd == "help") {
            showHelp();
        }

        else if (cmd == "ls") {
            listDir(currentPath);
        }
        else if (cmd == "cd") {
            cin >> arg1;
            if (chdir(arg1.c_str()) == 0) {
                char buf[500];
                getcwd(buf, sizeof(buf));
                currentPath = buf;
            } else {
                cout << "Directory not found!" << endl;
            }
        }
        else if (cmd == "new") {      // create file
            cin >> arg1;
            ofstream file(arg1);
            cout << "✅File created!" << endl;
        }
        else if (cmd == "del") {      // delete file
            cin >> arg1;
            remove(arg1.c_str());
            cout << "❌File deleted!" << endl;
        }
        else if (cmd == "ren") {      // rename
            cin >> arg1 >> arg2;
            rename(arg1.c_str(), arg2.c_str());
            cout << "Rename successful" << endl;
        }
        else if (cmd == "cp") {       // copy
            cin >> arg1 >> arg2;
            ifstream src(arg1, ios::binary);
            ofstream dest(arg2, ios::binary);
            dest << src.rdbuf();
            cout << "File copied" << endl;
        }
        else if (cmd == "search") {
            cin >> arg1;
            searchFile(currentPath, arg1);
        }
        else if (cmd == "chmod") {
            cout << "Enter FileName then Permission (e.g. 755): ";
            cin >> arg2>> arg1;

            try {
                chmod(arg2.c_str(), stoi(arg1, 0, 8));
                cout << "✅ Permission changed\n";
            } catch(...) {
                cout << "❌ Invalid permission format\n";
            }
        }
        else if (cmd == "clear") {
            system("clear");   // clears the terminal screen
        }

        else if (cmd == "exit") break;
    }
}