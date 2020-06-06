#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <minix/type.h>
int main(int argc, char* argv[]) {
    message mes;
    pid_t  pid = getpid();
    const int UPPER_BOUND = 1e8;
    int i, j;
    mes.m1_i1 =  4;
    mes.m1_i2=  atoi(argv[1]);
    mes.m1_i3=  pid;
    _syscall(MM, 78, &mes); 
    for(i=1; i<UPPER_BOUND; ++i);
    printf("Koniec zadania typu: %d", atoi(argv[1]));
    return 0;
}
