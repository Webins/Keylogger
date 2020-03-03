#include "../headers/Helper.hpp"


Helper::DateTime::DateTime(){
    time_t ms;
    time(&ms);
    struct tm * info = localtime(&ms);
    /*Variables that represent the time of the local computer*/
    D = info-> tm_mday;
    m = info-> tm_mon +1; // because it starts from 0 e.g. January = 0
    y = 1900 + info->tm_year;
    H = info -> tm_hour;
    M = info -> tm_min;
    S = info -> tm_sec;
}

Helper::DateTime::DateTime(int D, int m, int y, int H, int M, int S)
    :D(D), m(m), y(y), H(H), M(M), S(S){};

Helper::DateTime::DateTime(int D, int m, int y)
    :D(D), m(m), y(y), H(0), M(0), S(0) {};

Helper::DateTime Helper::DateTime::now() const{
    return DateTime();
}


std::string Helper::DateTime::GetDateString() const{
    return (std::string((DateTime::D)< 10 ? "0" : "") + ToString(DateTime::D) +
    std::string((DateTime::m < 10)? ".0" : ".") + ToString(DateTime::m) +
    "." + ToString(DateTime::y));
    /*DD.mm.YYYY*/
}

std::string Helper::DateTime::GetTimeString(const std::string &sep) const{
    return (std::string(DateTime::H < 10 ? "0" : "") + ToString(DateTime::H) + sep +
    std::string(DateTime::M < 10 ? "0" : "") + ToString(DateTime::M) + sep +
    std::string(DateTime::S < 10 ? sep : "") + ToString(DateTime::S));
    /*HH:MM:SS*/
}

std::string Helper::DateTime::GetDateTimeString(const std::string &sep) const {
    return (GetDateString() + " " + GetTimeString(sep));
}


void Helper::WriteAppLog(const std::string &s){
    std::ofstream file{"AppLog.txt", std::ios::app};
    if(!file){
        return;
    }
    file << std::setw(20) << std::left << Helper::DateTime().GetDateTimeString() << "-" <<
    std::left << s << std::endl;
    file.close();
}

