#include<stdio.h>
#include<signal.h>
#include<unistd.h>
//#include<stdlib.h>

int pid1,pid2;
int EndFlag=0;
int pf1=0;
int pf2=0;

void IntDelete()
{
	kill(pid1,16);
	kill(pid2,17);
}

void Int1()
{
	printf("Child process1 is killed by parent!\n");
	exit(0);
}

void Int2()
{
	printf("Child process2 is killed by parent!\n");
	exit(0);
}

main()
{
	int exitpid;
	if(pid1=fork())
	{
		if(pid2=fork())
		{
			signal(SIGINT,IntDelete);
			waitpid(-1,&exitpid,0);
			waitpid(-1,&exitpid,0);
			printf("Parent process is killed!\n");
			exit(0);
		}
		else
		{
			signal(SIGINT,SIG_IGN);
			signal(17,Int2);
			pause();
		}
	}
	else
	{
		signal(SIGINT,SIG_IGN);
		signal(16,Int1);
		pause();
	}
	//return 0;
}
