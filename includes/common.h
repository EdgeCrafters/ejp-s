#pragma once
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STRSIZE 1048576

#define BUFSIZE 512
#define CMDSIZE 512
#define URLSIZE 512

#define VALUESIZE 32

#define IDSIZE 16 
#define PWSIZE 16 
#define MAXOPT 16

int parseOpt(int argc, char *argv[], const char targetOpt[], const int optNum, char *optArg[]);
void userLogin(const char home[]);
void userLogout(const char home[]);
char* getExtension(char *target);

// 아래 함수들은 http.h 에 있는 함수들을 사용하는 함수입니다.
// 아래 함수들의 내용은 common.c 에 있습니다.

int getBias(int repoId, int hiddenCaseId);
int postResult(int repoId, int hiddenCaseId);
int getRepoId();
int getHiddenCaseId();
void getSessionInfo();
void cacheSessionId();