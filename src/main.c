#include "common.h"

int repo(int argc, char*argv[]);
int problem(int argc, char*argv[]);

int main(int argc, char*argv[])
{
	char command[CMDSIZE];
	if(argc < 2 || !strcpy(command,argv[1])){
		fprintf(stderr,"no command ...\n");
		basicInfo();
		exit(-1);
	}

	if (!strncmp(command, "problem", 7)) {
		fprintf(stderr, "use problem command : \n");
		return problem(argc, argv);
	} else if (!strncmp(command, "repo", 4)) {
		fprintf(stderr, "use repo command : \n");
		return repo(argc, argv);
	} else {
		fprintf(stderr, "Please check command.\n");
	}
	return 0;
}
