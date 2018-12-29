#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std; 
 
int main(int argc,char *argv[])
{
	pid_t pid1,pid2;        //进程标识符
	pid1 = fork();     //创建一个新的进程
	if(pid1<0)
	{
		cout<<"创建进程失败!";
		exit(1);
	}
	else if(pid1==0)   //如果pid为0则表示当前执行的是子进程
	{
		//lockf(1,1,0);
		//cout<<"b"<<endl;
		for(int i=0;i<20;i++)
			cout<<"b:子进程1 "<<"进程标识符: "<<getpid()<<endl;
		//lockf(1,0,0);
		//wait(0);
		exit(0);
	}
	else          //否则为父进程
	{
		pid2 = fork();//創建一個新的進
		if(pid2<0)
		{
			cout<<"创建进程失败！";
			exit(1);
		}
		else if(pid2==0)   //如果pid为0则表示当前执行的是子进程
		{
			//lockf(1,1,0);
			//cout<<"c"<<endl;
			for(int i=0;i<20;i++)
				cout<<"c:子进程2 "<<"进程标识符: "<<getpid()<<endl;
			//lockf(1,0,0);
			//wait(0);
			exit(0);
		}
		else          //否则为父进程
		{
			//lockf(1,1,0);
			//cout<<"a"<<endl;
			for(int i=0;i<20;i++)
				cout<<"a:父进程 "<<"进程标识符: "<<getpid()<<endl;
			//lockf(1,0,0);
			exit(0);
		}
	}
	return 0;
}
