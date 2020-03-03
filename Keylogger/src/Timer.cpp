#include "../headers/Timer.hpp"

const long Timer::max_calls = -1L;


 void Timer::sleep_and_run(void){
        std::this_thread::sleep_for(interval); // bloquea el thread por un cierto tiempo (intervalo)
        if(alive)
            get_funct()();//el primer parentesis llama a la funcion Function, el segundo llama a la funcion que Function retorna
}

void Timer::thread_function(){ //funcion que se pasara al thread
        if(call_number == max_calls)
            while(alive)
                sleep_and_run();
        else
            while(repeat_count--)
                sleep_and_run();
}


    //constructors

Timer::Timer(const std::function <void(void)> &f)
        :funct(f) {};

Timer::Timer(const unsigned long &interval_val,
          const std::function <void(void)> &funct_val,
          const long call_number_val)
          :interval(std::chrono::milliseconds(interval_val)),funct(funct_val), call_number(call_number_val){};

void Timer::start_timer(bool async){
        if(alive) //if is already running is ok
            return;
        alive = true; // if not go and run
        repeat_count = call_number;
        if(async)
            thread = std::thread(thread_function, this); //ejecutalo simultaneamente
        else
            this->thread_function(); //execute the funct
}

void Timer::stop_timer(void){
        alive = false;
        thread.join(); //Hace que no haya threads concurrentes al main thread
}


//getters and setters
bool Timer::get_alive(void) const{
        return alive;
}

void Timer::set_repeat_count(const long &r){
        if(alive) return;
        call_number = r;
}

void Timer::set_function(const std::function<void(void)> &f){
        funct = f;
}

long Timer::get_left_count(void) const {
        return repeat_count;
}

void Timer::set_left_count(const long &n){
    repeat_count= n;
}
long Timer::get_repeat_count(void) const{
        return call_number;
}

void Timer::set_interval(const unsigned long &i){
        if(alive) return;
        interval = std::chrono::milliseconds(i);
}

unsigned long Timer::get_interval() const {
        return interval.count();
}

const std::function<void(void)> & Timer::get_funct()const{
        return funct;
}
