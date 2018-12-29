#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<iostream>
using namespace std;
void waiting(),stop(),alarming();
int wait_mark;

int main(int argc,char *argv[])
{
	int p1,p2;
	if(p1=fork())  //Create subprocess1
	{
		if(p2=fork())    //Create subprocess2
		{
			wait_mark=1;
			signal(SIGINT,stop);  //Change to stop when getting the signal ^c
			signal(SIGALRM,alarming); //Getting SIGALRM
			waiting();
			kill(p1,10);  //Sending soft interrupt signal 10 to p1
			kill(p2,11);  //Sending soft interrupt signal 11 to p2

			wait(0);
			wait(0);
			cout<<"Parent process is killed!"<<endl;
			exit(0);
		}
		else
		{
			wait_mark=1;
			signal(11,stop);
			signal(SIGINT,SIG_IGN);  //Ignore ^c signal
			while(wait_mark!=0);
			lockf(1,1,0);
			cout<<"Child process2 id killed by parent!"<<endl;
			lockf(1,0,0);
			exit(0);
		}
	}
	else
	{
		wait_mark=1;
		signal(10,stop);
		signal(SIGINT,SIG_IGN);   //Ignore ^c signal
		while(wait_mark!=0);
		lockf(1,1,0);
		cout<<"Child process1 is killed by parent!"<<endl;
		lockf(1,0,0);
		exit(0);
	}
}

void waiting()
{
	sleep(5);
	if(wait_mark!=0)
		kill(getpid(),SIGALRM);
}

void alarming()
{
	wait_mark=0;
}

void stop()
{
	wait_mark=0;
}
