#include "common.h"
#include "http.h"


int parseOpt(int argc, char *argv[], const char targetOpt[], const int optNum,
             char *optArg[], char *caches[])
{
    int c, result = 0;
    int flags[MAXOPT] = {0};

    while ((c = getopt(argc, argv, targetOpt)) != -1)
        for (int i = 0; i < optNum; ++i)
            if (c != '?' && c == targetOpt[2 * i])
            {
                flags[i] = 1;
                strcpy(optArg[i], optarg);
                result += 1;
            }
            else if (c == '?' && optopt == targetOpt[i])
            {
                fprintf(stderr, "Option -%c requires \
						an argument.\n",
                        optopt);
                exit(EXIT_FAILURE);
            }

    for (int i = 0; i < optNum; ++i)
        if (flags[i] && caches[i])
        {
            int cache;
            if ((cache = open(caches[i], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXO | S_IRWXU)) < 0)
                goto cacheException;
            else if (write(cache, optArg[i], strlen(optArg[i])) < 0)
            {
                close(cache);
                goto cacheException;
            }
            close(cache);
        }
        else if (caches[i])
        {
            int cache;
            char buf[BUFSIZE];
            if ((cache = open(caches[i], O_RDONLY)) < 0 || read(cache, buf, BUFSIZE) < 0)
                goto exception;

            strcpy(optArg[i], buf);
        }
        else if (flags[i])
            continue;
        else
            goto exception;

    return result;

exception:
    fprintf(stderr, "Missing options require %d, receive %d\n", optNum, result);
    exit(EXIT_FAILURE);

cacheException:
    fprintf(stderr, "Wrong with cache\n");
    exit(EXIT_FAILURE);
}

void userLogin(const char home[])
{
	char userID[IDSIZE], userPW[PWSIZE];
	memset(userID,0,IDSIZE);memset(userPW,0,PWSIZE);
	
	printf("\nEnter %s ID :", home);
	scanf("%s", userID);

 	printf("Enter %s PW :", home);
 	scanf("%s", userPW);

	if(!login(home, userID,userPW)){
		fprintf(stdout,"Login Success!\n");
	}else{
		fprintf(stdout,"Try again");
		userLogin(home);
	}
}

void userLogout(const char home[])
{
	logout(home);
}

char* getExtension(char *target)
{
	int i;
	for(i = 0; target[i] != '.' && target != '\0'; ++i)
		;
	return target+i;
}

int getExecutablePath(char path[])
{
    if (!path)
        goto exception;

    char buf[PATHSIZE];
    ssize_t len = readlink("/proc/self/exe", buf, PATH_MAX);
    if (len != -1)
    {
        buf[len] = '\0';
        strcpy(path, dirname(buf));
        return 0;
    }

exception:
    return -1;
}

