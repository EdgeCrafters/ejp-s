#include "http.h"
#include "common.h"

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

    if (getReposHTTP(home) < 0) {
        fprintf(stderr, "Fail to get repo informations.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}