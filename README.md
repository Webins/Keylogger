# Keylogger

Este keylogger esta escrito en C++, puede usarse en Windows a partir de Windows 7 en adelante. 
Al compilar el codigo, es necesario activar el estandar C++11 y la bandera -mwindows


# Description: 

El keyloger permite capturar todas las teclas introducidad en el teclado, el cual guardara a un archivo escondido
dentro de la carpeta AppData. Este archivo estara encriptado y sera enviado al correo especificado cada cierto tiempo.

This keylogger is written in C++ and can be used on Windows 7,8,10 machines.
Be sure to enable C++11 standards and the -mwindows flag to compile it.

# Description: 
This keylogger captures keyboard strokes using system hooks. It writes the keystrokes to a log file hidden in 
the AppData folder. This log file is encrypted in case the user somehow locates the log.In addition an email can be sent 
on a timer with the log file attached.
