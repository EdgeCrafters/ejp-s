#pragma once
#include <curl/curl.h>
#include "sha256.h"
#include "common.h"
#include "test.h"

struct cookie{
	char data[BUFSIZE];
	int isStore;
};

int login(const char hoem[], const char id[], const char password[]);
int logout(const char home[]);
int intiRepo(const char home[], const char repoID[], char buffer[], size_t bufSize);


int showReposHTTP(const char home[]);
int getReposHTTP(const char home[], int repoId);
int getReposManager(const char home[]);
void deleteAllFile(const char dir_name[]);

int submitResultHTTP(const char home[], const char* output, int testcaseId);