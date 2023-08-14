#pragma once
#include <curl/curl.h>
#include "sha256.h"
#include "common.h"

struct cookie{
	char data[BUFSIZE];
	int isStore;
};

int login(const char hoem[], const char id[], const char password[]);
int logout(const char home[]);
int initRepo(const char home[], const char repoID[], char buffer[], size_t bufSize);

// common.h 참고하기
// 아래 함수들의 내용은 http.c 에 있습니다. 

int getBiasHTTP(int repoId, int hiddenCaseId);
int postResultHTTP(repoId, hiddenCaseId);
int getAllReposHTTP();
void showAllReposHTTP(char* user_host);
int getRepoIdHTTP();
int getHiddenCaseIdHTTP();
void getSessionInfoHTTP();
void cacheSessionIdHTTP();