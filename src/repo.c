#include "../includes/common.h"
#include <stdio.h>

// 필요한 옵션
// h: host 주소
static int show(int argc, char*argv[]) {
    char* user_host = (char*) malloc(128 * sizeof(char));
    if (argc == 5 && strcmp(argv[3], "h") == 0) {
        user_host = argv[4];
		userLogin(user_host);
		showAllRepos(user_host);
    } else {
        printf("Please see the command description typing \"%s %s \"\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }
    return 1;
}

int repo(int argc, char*argv[])
{
	char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		repoInfo();
		exit(-1);
	}

	if(!strncmp(command,"show",4)){
		printf("show!\n");
		show(argc, argv);
	}
	return 0;
}

