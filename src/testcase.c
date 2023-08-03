#include "../includes/common.h" 
/**
 * related with testcase module
*/

static int get(int argc, char*argv[]) {
    return 0;
}

int testcase(int argc, char*argv[]) {
    char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		testcaseInfo();
		exit(-1);
	}

    if (!strncmp(command, "get", 3)) {
        // TODO
        printf("get!\n");
        if (get(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    }
}