#include "http.h"

int showRepos(char home[]) {
    if (showReposHTTP(home) < 0) {
        fprintf(stderr, "Fail to receive repo informations.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int getRepos(char home[]) {
    if (showReposHTTP(home) < 0) {
        fprintf(stderr, "Fail to receive repo informations.\n");
        exit(EXIT_FAILURE);
    }

    if (getReposManager(home) < 0) {
        fprintf(stderr, "Fail to get repo informations.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int cleanRepos() {
    char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
    deleteAllFile(repo_path);
    return 0;
}