#include "CallOption.h"


int main(){

    
    CallOption call(0.2, 0.05, 100.0, 110.0, 1.0, 252);

    call.simulation();

    

    return 0;
}