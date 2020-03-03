#ifndef __HELPER__H
#define __HELPER__H

#include <ctime> // for mail-sending triggers etc
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
namespace Helper{
    template<typename T>  // to convert pretty much anything to string
    inline std::string ToString(const T &e){
        std::ostringstream os;
        os << e;
        return os.str();
    }
    struct DateTime{
        int D, m, y, H, M, S;
        DateTime();
        DateTime(int, int, int, int, int, int);
        DateTime(int, int, int);
        DateTime now() const;
        std::string GetDateString() const;
        std::string GetTimeString(const std::string & =":") const;
        std::string GetDateTimeString(const std::string & =":") const;
    };
    void WriteAppLog(const std::string &);
}
#endif // __HELPER__H_H
