#include <stdio.h>
#include <lib.h>
#include <stdlib.h>


int getprocnr(int proc_nr)
{
	message msg;
	msg.m1_i1 = proc_nr;
	return _syscall(MM_PROC_NR,GETPROCNR,&msg);
}

int main(int argc, char* argv[])
{
	int i;
	int result;

	if(argc == 1)
		printf("Argument needed!");
	else
	{
		int where_to_start = atoi(argv[1]);
		for(i=0; i< 11; i++)
		{
			result = getprocnr(where_to_start+i);
			if(result == -1)
				printf("Process with id:%d is not in use or it doesen't exist! Error: %d\n", where_to_start+i, errno);
			else
				printf("Process with id:%d is %d in a line\n", where_to_start+i, result);
				
		}
	}
	return 0;
}


