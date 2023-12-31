#pragma once
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <ctype.h>

#include <cJSON.h>

#define STRSIZE 1048576

#define BUFSIZE 512
#define CMDSIZE 512
#define URLSIZE 512
#define PATHSIZE 512

#define VALUESIZE 512

#define IDSIZE 16 
#define PWSIZE 16 
#define MAXOPT 16

extern char exe[PATHSIZE];
extern char homeCache[PATHSIZE];

int parseOpt(int argc, char *argv[], const char targetOpt[], const int optNum,
             char *optArg[], char *caches[]);
void userLogin(const char home[]);
void userLogout(const char home[]);
char* getExtension(char *target);
int getExecutablePath(char path[]);
