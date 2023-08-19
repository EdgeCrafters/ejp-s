#include "common.h"

char exe[PATHSIZE];
char homeCache[PATHSIZE];

int showRepos(char home[]);
int getRepos(char home[]);
int cleanRepos();

static int show(int argc, char*argv[]) {
	char home[VALUESIZE];
	char *values[] = {home};
	char *cache[] = {homeCache, NULL, NULL};

	if (parseOpt(argc, argv, "h:", 1, values, cache) < 1) {
		fprintf(stderr, "Please check options\n");
		exit(EXIT_FAILURE);
	}

	userLogin(home);
	showRepos(home);
	userLogout(home);
	return 0;
}

static int get(int argc, char*argv[]) {
	char home[VALUESIZE];
	char *values[] = {home};
	char *cache[] = {homeCache, NULL, NULL};

	if (parseOpt(argc, argv, "h:", 1, values, cache) < 1) {
		fprintf(stderr, "Please check options\n");
		exit(EXIT_FAILURE);
	}

	userLogin(home);
	getRepos(home);
	userLogout(home);
	return 0;
}

static int clean(int argc, char*argv[]) {
	cleanRepos();
	return 0;
}

int repo(int argc, char*argv[])
{
	char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		repoInfo();
		exit(-1);
	}

	if (getExecutablePath(exe) < 0) {
		fprintf(stderr, "Cannot configure current path.\n");
		exit(-1);
	} else {
		sprintf(homeCache, "%s/../.ejs/cache/home.txt", exe);
		fprintf(stderr, "home: %s\n", homeCache);
	}

	if (!strncmp(command, "show", 4)) {
		if (show(argc, argv)) {
			fprintf(stderr, "Error\n");
			exit(-1);
		}
	} else if (!strncmp(command, "get", 3)) {
		if (get(argc, argv)) {
			fprintf(stderr, "Error\n");
			exit(-1);
		}
	} else if (!strncmp(command, "clean", 5)) {
		if (clean(argc, argv)) {
			fprintf(stderr, "Error\n");
			exit(-1);
		}
	} else {
		fprintf(stderr, "Please check the command.\n");
		exit(-1);
	}

	return 0;
}

