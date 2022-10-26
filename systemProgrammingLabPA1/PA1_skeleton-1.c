#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int ls(char *dir_path, char *option);
int head(char *file_path, char *line);
int tail(char *file_path, char *line);
int mv(char* file_path1, char* file_path2);
int mk_dir(char *dir_path);
int rm_dir(char *dir_path);

#define MAX_CMD_LEN 200

int main(){
	while(1){
		int i, cmdret;
		char *cmd[MAX_CMD_LEN];
		char *argument[10];
	    size_t size;	
		
        /* Input commands */


        /* ============== */
        
        /* Tokenize commands (Use strtok function) */


        /* ======================================= */


        /* Command */

		if (strcmp("ls", argument[0]) == 0){
			cmdret = ls(argument[1], argument[2]);		
		}
		else if (strcmp("head", argument[0]) == 0){
			cmdret = head(argument[3], argument[2]);
		}
		else if (strcmp("tail", argument[0]) == 0){
			cmdret = tail(argument[3], argument[2]);
		}
		else if (strcmp("mv", argument[0]) == 0){
			cmdret = mv(argument[1], argument[2]);
		}
		else if (strcmp("mkdir", argument[0]) == 0){
			cmdret = mk_dir(argument[1]);
		}
		else if (strcmp("rmdir", argument[0]) == 0){
			cmdret = rm_dir(argument[1]));
		}
		else if (strcmp("quit", argument[0]) == 0){
			break;
		}
		else{
			/* Print "ERROR: Invalid commands" */

            continue;
		}

		if (cmdrlt == -1){
            /* Print "ERROR: The command is executed abnormally" */
		}
	}
	return 0;
}

int ls(char *dir_path, char *option){
	
}

int head(char *file_path, char *line){
	
}

int tail(char *file_path, char *line){
	
}

int mv(char *file_path1, char *file_path2){
	

}

int mk_dir(char *dir_path){
	
}

int rm_dir(char *dir_path){
	
}