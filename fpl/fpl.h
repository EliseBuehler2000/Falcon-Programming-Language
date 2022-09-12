object** modules=NULL;

#include "time.cpp"
#include "math.cpp"

typedef object* (*newmodulefunc)(void);
const size_t nmodules=2;
newmodulefunc newmodules[] = {(newmodulefunc)new_time_module, (newmodulefunc)new_math_module, NULL};

void setup_modules(){
    int i=0;
    modules=(object**)fpl_malloc(sizeof(object*)*nmodules);
    newmodulefunc mod=newmodules[i++];
    while (mod){
        modules[i-1]=mod();
        mod=newmodules[i++];
    }
}