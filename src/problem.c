#include "../includes/common.h" 
/**
 * related with problem module
*/

static int show(int argc, char*argv[]) {
    return 0;
}

static int get(int argc, char*argv[]) {
    return 0;
}

static int submit(int argc, char*argv[]) {
    return 0;
}

static int test(int argc, char*argv[]) {
    return 0;
}

int problem(int argc, char*argv[]) {
    char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		problemInfo();
		exit(-1);
	}

    if(!strncmp(command,"show",4)) {
        // TODO
        printf("show!\n");
        if (show(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "get", 3)) {
        // TODO
        printf("get!\n");
        if (get(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "submit", 6)) {
        // TODO
        printf("submit!\n");
        if (submit(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "test", 4)) {
        // TODO
        printf("test!\n");
        if (test(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    }
}