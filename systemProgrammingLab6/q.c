#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	printf("~");
	unsigned int pid;
	time_t t;
	struct tm *tm;	
	char *argv[3];
	char *bufLines[500];
	char buf[5120];
	int bufLineSize;
	int bufSize;
	int fd, fd0, fd1, fd2;
	int min, hour;
	printf("~");
	if(chdir("/") < 0)
		exit(0);
	fd = open("/crontab", O_RDWR);
	printf("~");

	t = time(NULL);
	tm = localtime(&t);
	printf("m : %d, h : %d\n", tm->tm_min, tm->tm_hour);
	printf("[2021313504] : crond for syslab6 assignment is now run\n");
	buf[0] = '\0';
	if ((bufSize = (read(fd, buf, sizeof(buf)))) <= 0) {
		printf("[2021313504] : Fail to read the file. (Please do on 'sudo' mode)\n");
	}
	else if (bufSize >= (sizeof(buf) - 1)) {
		printf("[2021313504] : The file is too big.\n");
	}
	else {
		bufLines[(bufLineSize = 0)] = strtok(buf, "\n");
		while ((bufLines[++bufLineSize] = strtok(NULL, "\n")));
		for (int i = 0; i < bufLineSize; i++){
			printf("%s\n", bufLines[i]);
		}
		for (int i = 0; i < bufLineSize; i++){
			argv[0] = strtok(bufLines[i], " ");
			if(argv[0] == NULL || argv[0][0] == '#') {
				printf("It is comment.\n");
			}
			if((argv[1] = strtok(NULL, " ")) == NULL) continue;
			if((argv[2] = strtok(NULL, " ")) == NULL) continue;

			if (argv[0] == '*') min = -1;
			else min = atoi(argv[0]);
			if (argv[1] == '*') hour = -1;
			else hour = atoi(argv[1]);
		}
		while (0)
		{

			//insert your code

			//##  hints  ##

			//strtok_r();
			//pid = fork();
			//execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);

			t = time(NULL);
			tm = localtime(&t);
	
			sleep(60 - tm->tm_sec % 60);
		}
	}
	return 0;
	
}
