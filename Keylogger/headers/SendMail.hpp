#ifndef __SENDMAIL__H
#define __SENDMAIL__H

#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include "IO.hpp"
#include "Helper.hpp"
#include "Timer.hpp"


#define script_name "sm.ps1"


namespace Mail{
    std::string replace_string(std::string s, const std::string &what, const std::string &with);
    bool check_file(const std::string &f);
    bool create_script();
    int send_mail(const std::string &subj, const std::string &body, const std::string &attachments);
    int send_mail(const std::string &subj, const std::string &body, const std::vector<std::string> &attachments);
}
#endif
