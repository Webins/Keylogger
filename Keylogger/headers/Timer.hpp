#ifndef __TIMER__H
#define __TIMER__H

#include <thread>
#include <chrono>
class Timer{
private:
    std::thread thread; // Indica el thread actual
    bool alive= false; // Indica si el thread esta vivo o no
    long call_number = -1L; // indica cuantas veces va a ser llamado una cierta funcion
    long repeat_count = -1L; //cuenta la cantidad de veces que una funcion se ha llamado
    std::chrono::milliseconds interval = std::chrono::milliseconds(0); // intervalo entre cada threads
    std::function<void(void)> funct = nullptr; // una funcion que actua como un pointer a una funcion

    void sleep_and_run(void);
    void thread_function(); //funcion que se pasara al thread

public:
    static const long max_calls;

    //constructors
    Timer() = default;
    Timer(const std::function <void(void)> &f);
    Timer(const unsigned long &interval_val,
          const std::function <void(void)> &funct_val,
          const long call_number_val = Timer::max_calls);

    void start_timer(bool async = true);
    void stop_timer(void);

    //getters and setters
    bool get_alive(void) const;
    void set_repeat_count(const long &r);
    void set_function(const std::function<void(void)> &f);
    long get_left_count(void) const;
    long get_repeat_count(void) const;
    void set_left_count(const long &);
    void set_interval(const unsigned long &i);
    unsigned long get_interval() const;
    const std::function<void(void)> & get_funct() const;


};



#endif
