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
	int pid;
	time_t t;
	struct tm *tm;	
	char *argv[3] = { 0 };
	char *bufLines[500];
	char buf[5120] = { 0 };
	int bufLineSize;
	int bufSize;
	int fd, fd0, fd1, fd2;
	int min = -1, hour = -1;
	pid = fork();
	
	if(pid == -1) return -1;
	if(pid != 0)
		exit(0);
	if(setsid() < 0)
		exit(0);
	if(chdir("/") < 0)
		exit(0);
	
	fd = open("/crontab", O_RDWR);

	umask(0);

	close(0);
	close(1);
	close(2);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = open("/dev/null", O_RDWR);
	fd2 = open("/dev/null", O_RDWR);

	t = time(NULL);
	tm = localtime(&t);
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog("myCron", LOG_PID, LOG_CRON);
	
	syslog(LOG_DEBUG, "[2021313504] : crond for syslab6 assignment is now run");
	buf[0] = '\0';
	if ((bufSize = (read(fd, buf, sizeof(buf)))) <= 0) {
		syslog(LOG_ERR, "[2021313504] : Fail to read the file. (Please do on 'sudo' mode)");
	}
	else if ((bufSize + 1) >= (int)sizeof(buf)) {
		syslog(LOG_ERR, "[2021313504] : The file is too big.");
	}
	else {
		bufLines[(bufLineSize = 0)] = strtok(buf, "\n");
		while ((bufLines[++bufLineSize] = strtok(NULL, "\n")));
		for (int i = 0; i < bufLineSize; i++){
			argv[0] = strtok(bufLines[i], " ");
			if (argv[0] == NULL || argv[0][0] == '#') continue;
			if ((argv[1] = strtok(NULL, " ")) == NULL) continue;
			if ((argv[2] = strtok(NULL, " ")) == NULL) continue;

			if (argv[0][0] == '*') min = -1;
			else min = atoi(argv[0]);
			if (argv[1][0] == '*') hour = -1;
			else hour = atoi(argv[1]);
		}
		while (1)
		{
			if ((min == -1 || min == tm->tm_min) && (hour == -1 || hour == tm->tm_hour)){
				if ((pid = fork()) == 0){
					wait(NULL);
				}
				else {
					execl("/bin/sh", "bin/sh", "-c", argv[2]);
					syslog(LOG_DEBUG, "[2021313504] : cron success.");
					syslog(LOG_DEBUG, argv[2]);
				}
			}
			t = time(NULL);
			tm = localtime(&t);
			sleep(60 - tm->tm_sec % 60);
		}
	}
    closelog();
	return 0;
}
