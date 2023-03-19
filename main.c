#include "head.h"


int main()
{
    pro_init();
    readtxt();    
    handle_pthread();
    pro_free();        
    
    return 0;
}