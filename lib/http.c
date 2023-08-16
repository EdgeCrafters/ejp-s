#include "http.h"
#include "../includes/cJSON.h"

struct cookie session = {.isStore = 0};
int option = 0;

static size_t plainWrite(void *data, size_t size, size_t nmemb, void *clientp)
{
	if(clientp)
		strncpy((char*)clientp, (char*)data, nmemb);
	return size*nmemb;
}

static void deleteAllFile(const char* dir_name) {
	DIR *dir = opendir(dir_name);

	if (dir == NULL) {
		perror("Error opening directory.\n");
		return ;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_REG) {
			char file_path[1024];
			snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, entry->d_name);

			if (remove(file_path) != 0) {
				perror("Error deleting file.\n");
			} else {
				printf("Deleted file: %s\n", file_path);
			}
		}
	}

	closedir(dir);
}

static void makeReposJsonFile(int numItems, cJSON *contentArray, char *dir_name) {
	for (int i=0; i<numItems; i++) {
		cJSON *item = cJSON_GetArrayItem(contentArray, i);
		cJSON *json_obj = cJSON_Print(item);
		char file_path[512];

		if (item) {
			cJSON *repoId = cJSON_GetObjectItem(item, "repoId");
			cJSON *repoName = cJSON_GetObjectItem(item, "repoName");

			// json 파일 이름 생성 형식 "myRepos/repoId_repoName.json"
			sprintf(file_path, "%s/%d_%s.json", dir_name, repoId->valueint, repoName->valuestring);
			FILE *json_file = fopen(file_path, "w");

			if (json_file) {
				fprintf(json_file, "%s\n", json_obj);
				printf("Success to make json file named %s\n", file_path);
				fclose(json_file);
			} else {
				printf("Failed to make json file...\n");
			}
		}
	}
}
// option 이 0일 경우에는 보여주기만 하고 1일 경우에는 해당 repo 들에 대한 정보를 담고 있는 json 파일을 만들어줍니다.
static size_t reposResponse(void* contents, size_t size, size_t nmemb, void* clientp) {
	size_t totalSize = size * nmemb;
	cJSON *response = cJSON_Parse((const char*) contents);	// json 응답결과 파싱

	if (response) {
		cJSON *contentArray = cJSON_GetObjectItem(response, "content");

		if (contentArray) {
			int numItems = cJSON_GetArraySize(contentArray);

			if (option == 1) {
				const char dir_name[12] = "../myRepos";

				struct stat dir_info;
				if (stat(dir_name, &dir_info) == 0) {
					if (S_ISDIR(dir_info.st_mode)) { // 이전에 한번 받아온 경우
						deleteAllFile(dir_name);
						makeReposJsonFile(numItems, contentArray, dir_name);
					} else {
						printf("Path exists, but it's not a dricetory.\n");
						return 0;
					} 
				} else {
					if (mkdir(dir_name, 0755) == 0) { // 새로 받아오는 경우
						makeReposJsonFile(numItems, contentArray, dir_name);
					} else {
						printf("Error creating directory.\n");
						return 0;
					}
				}
			} else {
				printf("\n\nInformation of your repositories: \n");
				printf("%-20s %-20s\n", "repositoryId", "repositoryName");
				for (int i=0; i<numItems; i++) {
					cJSON *item = cJSON_GetArrayItem(contentArray, i);

					if (item) {
						cJSON *repoId = cJSON_GetObjectItem(item, "repoId");
						cJSON *repoName = cJSON_GetObjectItem(item, "repoName");

						if (repoId && repoName) {
							printf("%-20d %-20s\n", repoId->valueint, repoName->valuestring);
						}
					}
				}
			}
		} else {
			printf("Your repositories are empty. Please contact the Tutor.\n");
			return 0;
		}
	} else {
		printf("Failed to request...\n");
		return 0;
	}

	cJSON_Delete(response);

	return totalSize;
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

// - - - - submit 관련 - - - - 

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

// - - - - submit 관련 - - - -


// - - - - repo 관련 - - - -

// repoId 값을 반환합니다.
int getRepoIdHTTP() {
	// TODO
	// test 용 repoId
	return 1;
}

// 현재 이용가능한 repo 정보들을 가져와서 directory 로 만들어둡니다.
int getAllReposHTTP(char* user_host) {
	// TODO
	option = 1;
	char url[URLSIZE];
	char buf[512];
	sprintf(buf, "Cookie: %s", session.data);	// sessionId 설정
	CURL* curl;
	CURLcode response;
	struct curl_list *list = NULL;
	long stat;

	// url 합치기
	memset(url, 0, URLSIZE);
	sprintf(url, "%s/repos", user_host);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
		list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, buf);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reposResponse);

		response = curl_easy_perform(curl);

		if (response != CURLE_OK) 
			return 0;

		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, "Error on curl...\n");
		return 0;
	}

	return 1;
}

// 현재 이용가능한 repo 정보들을 출력합니다. 
void showAllReposHTTP(char* user_host) {
	// TODO
	option = 0;
	char url[URLSIZE];
	char buf[512];
	sprintf(buf, "Cookie: %s", session.data);	// sessionId 설정
	CURL* curl;
	CURLcode response;
	struct curl_list *list = NULL;
	long stat;

	// url 합치기
	memset(url, 0, URLSIZE);
	sprintf(url, "%s/repos", user_host);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
		list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, buf);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reposResponse);

		response = curl_easy_perform(curl);

		if (response != CURLE_OK) 
			return;

		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, "Error on curl...\n");
		return;
	}
}

// - - - - repo 관련 - - - -


// - - - - testcase 관련 - - - -

// hiddenCaseId 값을 반환합니다.
int getHiddenCaseIdHTTP() {
	// TODO
	// test 용 hiddenCaseId
	return 1;
}

// - - - - testcase 관련 - - - -

// 로그인 시 session 정보를 출력합니다.
void getSessionInfoHTTP() {
	printf("session ID: %s\n", session.data);
	printf("isStore: %d\n", session.isStore);
}

// sessionId 를 캐싱합니다.
void cacheSessionIdHTTP() {
	// TODO
}