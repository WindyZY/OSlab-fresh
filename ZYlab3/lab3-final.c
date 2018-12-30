#include "sys/types.h"
#include "sys/file.h"
#include "unistd.h"

char produce_buf[4];
char consume_buf[4];

int pipe_fd[2];

pid_t pid1,pid2,pid3,pid4;
int producer(int id);
int consumer(int id);
int main(int argc,char **argv)
{ 
	if(pipe(pipe_fd)<0) 
	{ 
		printf("pipe create error \n"); 
		exit(-1); 
	}
	else
	{ 
		printf("pipe is created successfully!\n"); 
		if((pid1=fork())==0) 
			producer(1); 
		if((pid2=fork())==0) 
			producer(2); 
		if((pid3=fork())==0) 
			consumer(1); 
		if((pid4=fork())==0) 
			consumer(2); 
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	int i,pid,status; 
	for(i=0;i<4;i++) 
		pid=wait(&status); 
	exit(0); 
} 
int producer(int id) 
{
	printf("producer %d is running!\n",id); 
	close(pipe_fd[0]); 
	int i=0; 
	for(i=1;i<10;i++) 
	{
		sleep(3);
		if(id==1)
			strcpy(produce_buf,"A\0");
		else
			strcpy(produce_buf,"a\0");
		
