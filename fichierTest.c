#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 80000
#define clocksPerMiliSec 1000/CLOCKS_PER_SEC

clock_t start,end;
int ticks;
int delta;
char* a=0;

int main (void){
    start=clock();
    for (int i=0;i<N;++i){
        a = a+i;
        for (int j=0;j<N;++j){a = a+j;}
    }
    end=clock();
    ticks=end-start;
    delta=ticks*clocksPerMiliSec;
    printf("%ldCPS\n",CLOCKS_PER_SEC);
    printf("%dticks\n",ticks);
    printf("%dms\n",delta);
}
