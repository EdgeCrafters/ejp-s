#include "http.h"

struct cookie session = {.isStore = 0};

static size_t plainWrite(void *data, size_t size, size_t nmemb, void *clientp)
{
	if(clientp)
		strncpy((char*)clientp, (char*)data, nmemb);
	return size*nmemb;
}

size_t storeCookie(char *buffer, size_t size, size_t nitems, void *userdata)
{
	if(!strncmp(buffer,"Set-Cookie",10)){
		int i;
		for(i = 0; buffer[i]!=';'; ++i)
			;
		buffer[i] = '\0';
		sprintf(session.data,"%s",buffer+12);
		session.isStore = 1;
	}
	return nitems * size;
}

int login(const char home[], const char id[], const char pw[])
{
	char url[URLSIZE], payload[URLSIZE];
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url,0,URLSIZE);
	sprintf(url,"%s/auth/login",home);

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list,"Accept: */*");
		list = curl_slist_append(list,"Content-Type: application/json");
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);

		sprintf(payload,"{\"username\": \"%s\",\"password\":\"%s\"}",id,pw);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,payload);
		
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, storeCookie);
		
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, plainWrite);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			return -1;

		curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &stat);
		if(stat != 201)
			return -1;

		curl_easy_cleanup(curl);
	}else{
		fprintf(stderr,"Error on curl...\n");
		return -1;
	}
	return 0;
}

int logout(const char home[])
{
	char url[URLSIZE],cookie[BUFSIZE];
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url,0,URLSIZE);
	sprintf(url,"%s/auth/logout",home);

	memset(cookie,0,BUFSIZE);
	sprintf(cookie,"Cookie: %s",session.data);

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

		list = curl_slist_append(list, cookie);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,"");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, plainWrite);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			return -1;
		
		curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &stat);
		if(stat != 201)
			return -1;

		curl_easy_cleanup(curl);
	}else{
		fprintf(stderr,"Error on curl...\n");
	}
	return 0;
}

int initRepo(const char home[], const char repoID[], char buffer[], size_t bufSize) {

	char url[URLSIZE],cookie[BUFSIZE];
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url,0,URLSIZE);
	sprintf(url,"%s/repos/%s",home,repoID);

	memset(cookie,0,BUFSIZE);
	sprintf(cookie,"Cookie: %s",session.data);

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

		list = curl_slist_append(list,cookie);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,"");
		
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)buffer);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, plainWrite);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			return -1;

		curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &stat);
		if(stat != 201)
			return -1;

		curl_easy_cleanup(curl);

	}else{
		fprintf(stderr,"Error on curl...\n");
	}

	return 0;
}

// repoId 와 hiddenCaseId 를 기반으로 bias 값을 반환합니다.
int getBiasHTTP(int repoId, int hiddenCaseId) {
	// TODO
	// test 용 bias
	return 27;
}

// repoId 와 hiddenCaseId 를 기반으로 hiddenCase 를 맞추었는지 여부를 확인합니다.
int postResultHTTP(int repoId, int hiddenCaseId) {
	// TODO
	return 1;
}

// repoId 값을 반환합니다.
int getRepoIdHTTP() {
	// TODO
	// test 용 repoId
	return 1;
}

// hiddenCaseId 값을 반환합니다.
int getHiddenCaseIdHTTP() {
	// TODO
	// test 용 hiddenCaseId
	return 1;
}

// 로그인 시 session 정보를 출력합니다.
void getSessionInfoHTTP() {
	printf("session ID: %s\n", session.data);
	printf("isStore: %d\n", session.isStore);
}

// sessionId 를 캐싱합니다.
void cacheSessionIdHTTP() {
	// TODO
}