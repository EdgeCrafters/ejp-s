#include "http.h"

struct cookie session = {.isStore = 0};
unsigned int writeidx;
unsigned int repoIds[10000];
unsigned int cnt = 0;


static size_t plainWrite(void *data, size_t size, size_t nmemb, void *clientp)
{
    if (clientp)
    {
        strncpy((char *)clientp + writeidx, (char *)data, nmemb);
        writeidx += nmemb;
    }
    return size * nmemb;
}

static size_t writeCallback(void *data, size_t size, size_t nmemb, void *clientp) {
    char *response = (char *)clientp;

    // 현재 응답 데이터를 response에 연결
    strcat(response, (char *)data);

    return size * nmemb;
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
		curl_easy_setopt(curl, CURLOPT_PORT, 4000L);
		
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
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
		curl_easy_setopt(curl, CURLOPT_PORT, 4000L);

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

static int showReposResponse(cJSON* response) {
	cnt = 0;
	cJSON* message = cJSON_GetObjectItem(response, "message");

	if (!strcmp(message->valuestring, "success")) {
		cJSON* content = cJSON_GetObjectItem(response, "content");
		int size = cJSON_GetArraySize(content);

		if (content && size > 0) {
			fprintf(stderr, "\n\nInformation of your repositories: \n");
			fprintf(stderr, "%-20s %-20s\n", "repositoryId", "repositoryName");

			for (int i=0; i<size; i++) {
				cJSON* item = cJSON_GetArrayItem(content, i);

				if (item) {
					cJSON* id = cJSON_GetObjectItem(item, "repoId");
					cJSON* name = cJSON_GetObjectItem(item, "repoName");
					repoIds[cnt++] = id->valueint;

					if (id && name) {
						fprintf(stderr, "%-20d %-20s\n", id->valueint, name->valuestring);
					}
				}
			}
		} else {
			fprintf(stderr, "The repositories do not exist.\n");
			return -1;
		}
	} else {
		fprintf(stderr, "%s\n", message->valuestring);
		return -1;
	}

	return 0;
}

static int getReposResponse(cJSON* response) {
	// const char* repo_path = (char*) malloc(sizeof(char));
	char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
	struct stat dir_info;

	if (stat(repo_path, &dir_info) == 0) {
		if (S_ISDIR(dir_info.st_mode)) {
			makeReposJsonFile(response, repo_path);
		} else {
			fprintf(stderr, "Path exist, but it's not a directory.\n");
			return -1;
		}
	} else {
		if (mkdir(repo_path, 0755) == 0) {
			makeReposJsonFile(response, repo_path);
		} else {
			fprintf(stderr, "Error on creating directory.\n");
			return -1;
		}
	}

	return 0;
}

static size_t submitResultResponse(void *data, size_t size, size_t nmemb, void *clientp) {
	cJSON *response = cJSON_Parse((const char*) data);

	if (response) {
		cJSON *message = cJSON_GetObjectItem(response, "message");
		fprintf(stderr, "%s\n", message->valuestring);
	} else {
		fprintf(stderr, "Fail to request.\n");
		return -1;
	}

	return size * nmemb;
}

void makeReposJsonFile(cJSON *data, char dir_name[]) {
	cJSON *repoId = cJSON_GetObjectItem(data, "id");
	cJSON *repoName = cJSON_GetObjectItem(data, "name");
	cJSON *json_obj = cJSON_Print(data);
	char file_path[512];

	sprintf(file_path, "%s/%d_%s.json", dir_name, repoId->valueint, repoName->valuestring);
	FILE *json_file = fopen(file_path, "w");

	if (json_file) {
		fprintf(json_file, "%s\n", json_obj);
		printf("Success to make json file named %s\n", file_path);
		fclose(json_file);
	} else {
		fprintf(stderr, "Fail to make json file.\n");
		return ;
	}
}

void deleteAllFile(const char dir_name[]) {
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

int showReposHTTP(const char home[]) {
	char url[URLSIZE], cookie[BUFSIZE], response[STRSIZE] = {'\0'};
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url, 0, URLSIZE);
	sprintf(url, "%s/repos", home);

	memset(cookie, 0, BUFSIZE);
	sprintf(cookie, "Cookie: %s", session.data);

	curl = curl_easy_init();

	if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_PORT, 4000L);

        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
        list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, cookie);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return -1;
		}

		cJSON* json = cJSON_Parse(response);
		if (showReposResponse(json) < 0) {
			return -1;
		}

		curl_easy_cleanup(curl);
	} else {
		return -1;
	}

	return 0;
}

int getReposHTTP(const char home[], int repoId) {
	char url[URLSIZE], cookie[BUFSIZE], response[STRSIZE] = {'\0'};
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url, 0, URLSIZE);
	sprintf(url, "%s/repos/%d", home, repoId);

	memset(cookie, 0, BUFSIZE);
	sprintf(cookie, "Cookie: %s", session.data);

	curl = curl_easy_init();

	if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_PORT, 4000L);

        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
        list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, cookie);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return -1;
		}

		cJSON* json = cJSON_Parse(response);

		if (getReposResponse(json) < 0) {
			return -1;
		}

		curl_easy_cleanup(curl);
	} else {
		return -1;
	}

	return 0;
}

int getReposManager(const char home[]) {
	for (int i=0; i<cnt; i++) {
		if (getReposHTTP(home, repoIds[i]) < 0) {
			return -1;
		}
	}
	return 0;
}

int submitResultHTTP(const char home[], const char* output, int testcaseId) {
	char url[URLSIZE], cookie[BUFSIZE], response[BUFSIZE], payload[STRSIZE];
	char *buf = (char*) malloc(512*sizeof(char));
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;
	long stat;

	memset(url, 0, URLSIZE);
	sprintf(url, "%s/testcase/%d", home, testcaseId);

	memset(cookie, 0, BUFSIZE);
	sprintf(cookie, "Cookie: %s", session.data);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_PORT, 4000L);
		
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
        list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, cookie);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        size_t output_length = strlen(output);
        while (output_length > 0 && (output[output_length - 1] == ' ' || output[output_length - 1] == '\t' || output[output_length - 1] == '\n' || output[output_length - 1] == '\r')) {
            output_length--;
        }
		sprintf(buf, "%.*s", output_length, output);
		buf = SHA256(buf);

		sprintf(payload, "{\"hashedOutput\": \"%s\"}", buf);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(payload));

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, submitResultResponse);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return -1;
		}

		curl_easy_cleanup(curl);
	} else {
		return -1;
	}

	return 0;
}

