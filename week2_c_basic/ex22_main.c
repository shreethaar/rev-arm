#include "ex22.h"
#include "dbg.h"

const char* MY_NAME = "Zed A. Shaw";

void scope_demo(int count) {
    log_info("count is: %d",count);

    if(count>10) {
        int count=100; // BAD! BUGS! 
        log_info("count in this scope is %d",count);
    }

    log_info("count is at exit: %d",count);
    count=3000;
    log_info("count after assign: %d", count);
}

int main(int argc, char* argv[]) {
    //test OUT THE_AGE accessors    

    log_info("My name: %s, age: %d", MY_NAME,get_age());
    set_age(100);
    log_info("My age is now: %d", THE_S
