#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <linux/sem.h>
#include <sys/types.h>

#define MAX_PRODUCTS 4
struct sembuf buf;

int down(int sem_id)
{
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	if (semop(sem_id, &buf, 1) == -1)
	{
		perror("Down failed!/n");
		return 0;
	}
	return 1;
}
int up(int sem_id)
{
	buf.sem_num = 0;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;
	if (semop(sem_id, &buf, 1) == -1)
	{
		perror("Up failed!/n");
		return 0;
	}
	return 1;
}

void main()
{
	union semun arg;
	pid_t producer1, producer2;
	pid_t consumer1, consumer2, consumer3;

	int mutex;
	int full;
	int empty;
	char *buffer;

	// 共享内存
	buffer = (char*) mmap(NULL, sizeof(char) * MAX_PRODUCTS,
	PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	int i;
	for (i = 0; i < MAX_PRODUCTS; i++)
		buffer[i] = ' ';

	// 创建信号量
	mutex = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	full = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	empty = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	arg.val = 1;
	if (semctl(mutex, 0, SETVAL, arg) == -1)
	{
		perror("set the mutex error\n");
		return;
	}
	arg.val = 0;
	if (semctl(full, 0, SETVAL, arg) == -1)
	{
		perror("set the full error\n");
		return;
	}
	arg.val = MAX_PRODUCTS;
	if (semctl(empty, 0, SETVAL, arg) == -1)
	{
		perror("set the empty error/n");
		return;
	}

	if ((producer1 = fork()) == 0)
	{
		// 写入大写字母
		while (1)
		{
			down(empty);
			down(mutex);
			srand(time(NULL));
			int offset = rand() % 26;
			char item = 'A' + offset;
			int j;
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				if (buffer[j] == ' ')
				{
					buffer[j] = item;
					printf("Producer P1 insert letter: %c\n", item);
					break;
				}
			}
			up(mutex);
			up(full);
			sleep(1);
		}
	}
	else if ((producer2 = fork()) == 0)
	{
		// 写入小写字母
		while (1)
		{
			down(empty);
			down(mutex);
			srand(time(NULL));
			int offset = rand() % 26;
			char item = 'a' + offset;
			int j;
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				if (buffer[j] == ' ')
				{
					buffer[j] = item;
					printf("Producer P2 insert letter: %c\n", item);
					break;
				}
			}
			up(mutex);
			up(full);
			sleep(1);
		}
	}
	else if ((consumer1 = fork()) == 0)
	{
		// 消费大写字母
		while (1)
		{
			down(full);
			down(mutex);
			char item;
			int j;
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				if (buffer[j] >= 'A' && buffer[j] <= 'Z')
				{
					item = buffer[j];
					buffer[j] = ' ';
					break;
				}
			}
			printf("Consumer C1 removes letter: %c\n", item);
			up(mutex);
			up(empty);
			sleep(2);
		}
	}
	else if ((consumer2 = fork()) == 0)
	{
		// 消费小写字母
		while (1)
		{
			down(full);
			down(mutex);
			char item;
			int j;
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				if (buffer[j] >= 'a' && buffer[j] <= 'z')
				{
					item = buffer[j];
					buffer[j] = ' ';
					break;
				}
			}
			printf("Consumer C2 removes letter: %c\n", item);
			up(mutex);
			up(empty);
			sleep(2);
		}
	}
	else if ((consumer3 = fork()) == 0)
	{
		// 大小写字母
		while (1)
		{
			down(full);
			down(mutex);
			char item;
			int j;
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				if (buffer[j] != ' ')
				{
					item = buffer[j];
					buffer[j] = ' ';
					break;
				}
			}
			printf("Consumer C3 removes letter: %c\n", item);
			up(mutex);
			up(empty);
			sleep(2);
		}
	}
	else
	{
		// 主进程
		while (1)
		{
			int j;
			printf("Buffer: ");
			for (j = 0; j < MAX_PRODUCTS; j++)
			{
				printf("%c ", buffer[j]);
			}
			printf(" \n");
			sleep(10);
		}
	}
	exit(0);
}
