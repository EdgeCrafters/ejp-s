#include "../includes/common.h"

int problem(int argc, char*argv[]);
int testcase(int argc, char*argv[]);
int repo(int argc, char*argv[]);

int main(int argc, char*argv[])
{
	char command[CMDSIZE];
	if(argc < 2 || !strcpy(command,argv[1])){
		fprintf(stderr,"no command ...\n");
		basicInfo();
		exit(-1);
	}

	if (!strncmp(command, "problem", 7)) {
		printf("use problem command : \n");
		return problem(argc, argv);
	} else if (!strncmp(command, "testcase", 8)) {
		printf("use testcase command : \n");
		return testcase(argc, argv);
	} else if (!strncmp(command, "repo", 4)) {
		printf("use repo command : \n");
		return repo(argc, argv);
	}
	return 0;
}
