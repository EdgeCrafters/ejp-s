#include "common.h" 
#include "cJSON.h"

int showProblems();
int testProblem();
int submitResult(char home[], char location[]);
int showTestcases(cJSON** testcasesPtr);

static int show() {
    if (showProblems() < 0) {
        return -1;
    }
    return 0;
}

static int submit(int argc, char*argv[]) {
    char home[VALUESIZE];
    char location[VALUESIZE];
    char *values[] = {home, location};
    char *cache[] = {homeCache, NULL, NULL};

    if (parseOpt(argc, argv, "h:l:", 2, values, cache) < 2) {
        fprintf(stderr, "Please check options\n");
        exit(EXIT_FAILURE);
    }

    userLogin(home);
    if (submitResult(home, location) < 0) {
        userLogout(home);
        return -1;
    }
    userLogout(home);

    return 0;
}

static int test() {
    return 0;
}

static int testcase() {
    if (showTestcases(NULL) < 0) {
        return -1;
    }
    return 0;
}

int problem(int argc, char*argv[]) {
    char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		problemInfo();
		exit(-1);
	}

	if (getExecutablePath(exe) < 0) {
		fprintf(stderr, "Cannot configure current path.\n");
		exit(-1);
	} else {
		sprintf(homeCache, "%s/../.ejs/cache/home.txt", exe);
		fprintf(stderr, "home: %s\n", homeCache);
	}

    if(!strncmp(command,"show",4)) {
        if (show()) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "submit", 6)) {
        if (submit(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strcmp(command, "test")) {
        // TODO
        printf("test!\n");
    } else if (!strcmp(command, "testcase")) {
        if (testcase()) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else {
        fprintf(stderr, "Please check command.\n");
        exit(-1);
    }
}