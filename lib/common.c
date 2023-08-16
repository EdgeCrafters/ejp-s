#include "common.h"
#include "http.h"


int parseOpt(int argc, char *argv[], const char targetOpt[], const int optNum, char *optArg[])
{
	int c, result = 0;
	int flags[MAXOPT] = {0};
	
	while((c = getopt(argc,argv,targetOpt))!=-1)
		for(int i = 0; i < optNum; ++i)
			if(c != '?' && c == targetOpt[2*i]){
				flags[i] = 1;
				strcpy(optArg[i],optarg);
				result += 1;
			}else if(c == '?' && optopt == targetOpt[i]){
				fprintf(stderr, "Option -%c requires \
						an argument.\n", optopt);
				exit(EXIT_FAILURE);
			}

	for(int i = 0, check = 1; i<optNum; check *= flags[i++])
		if(!check){
			fprintf(stderr,"Missing options require %d, receive %d\n", check, result);
			exit(EXIT_FAILURE);
		}

	return result;
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

// http.c 에 있는 getBiasHTTP() 를 호출합니다.
int getBias(int repoId, int hiddenCaseId) {
	// TODO
	// test 용 bias
	return getBiasHTTP(repoId, hiddenCaseId);
}

// http.c 에 있는 postResultHTTP() 를 호출합니다.
int postResult(int repoId, int hiddenCaseId) {
	// TODO
	return postResultHTTP(repoId, hiddenCaseId);
} 

// http.c 에 있는 getAllReposHTTP() 를 호출합니다.
int getAllRepos(char* user_host) {
	return getAllReposHTTP(user_host);
}

// http.c 에 있는 showAllReposHTTP() 를 호출합니다.
void showAllRepos(char* user_host) {
	return showAllReposHTTP(user_host);
}

// http.c 에 있는 getRepoIdHTTP() 를 호출합니다.
int getRepoId() {
	// TODO
	// test 용 repoId
	return getRepoIdHTTP();
}

// http.c 에 있는 getHiddenCaseIdHTTP() 를 호출합니다.
int getHiddenCaseId() {
	// TODO
	// test 용 hiddenCaseId
	return getHiddenCaseIdHTTP();
}

// http.c 에 있는 getSessionInfoHTTP() 를 호출합니다.
void getSessionInfo() {
	getSessionInfoHTTP();
}

void cacheSessionId() {
	cacheSessionIdHTTP();
}