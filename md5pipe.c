#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <sys/uio.h>

#include <sys/types.h>

#include <unistd.h>

#include <signal.h>

#include <sys/fcntl.h>
#include "md5.h"

static int buff[2];
static pid_t pid;
void finale_answer(int sig)
{
	if (sig == SIGINT)
	{
		char* res = (char*)malloc(33 * sizeof(char));
		int num = read(buff[0], res, 32);
		if (num == 32)
		{
			printf("encrypted by process %d : %s \n",pid,res);
			free(res);
		}
	}
}

void checkpipeconnection(int* pipe)//check if the pipe suceeded
{
	if (pipe < 0)
	{
		printf("pipe faild");
		exit(1);
	}
}


int main()
{
	int pipeval;
	pid_t father = getpid();


	pipeval = pipe(buff);
	checkpipeconnection(&pipeval);//function thar check if pipe failed 

	if (signal(SIGINT, finale_answer) == SIG_ERR) {
		printf("\n ERROR---> signal function failed \n");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0) //child
	{
		char* laststring = (char*)malloc(20 * sizeof(char));//the string that will be read from buf
		int rea = read(buff[0], laststring, 20);
		char mahrozet[32]; //the string that will be with the md5
		strcpy(mahrozet, md5(laststring).c_str());
		write(buff[1], mahrozet, strlen(mahrozet));
		kill(father, SIGINT); //wait for father signal
		free(laststring);
		pause();
	}




	else if (pid > 0) //father
	{
		char str[100]; //input
		printf("plain text: ");
		scanf("%s", str);
		int length = strlen(str);
		if (length > 20) //check if the string is bigger than 20
		{
			printf("string is too long\n");
			exit(EXIT_FAILURE);
		}
		char user_in[20];
		strcpy(user_in, str);
		int size = strlen(user_in);
		write(buff[1], user_in, strlen(user_in));
		pause(); //wait for chiled signal
		kill(pid, SIGKILL);
		exit(EXIT_SUCCESS);
	}
	return 0;
}


