#include "http.h"
#include "common.h"

cJSON* getProblems(char* dir_path, int repoId);
// cJSON* getTestCases(char* dir_path, int repoId, int problemId);
cJSON* getTestCases(cJSON* problems, int problemId);
cJSON* getTestCase(cJSON* testcases, int testcaseId);

int showProblems() {
    const char* dir_path = "../myRepos";
    int repoId = 0, problemCount = 0;
    cJSON* problems = NULL;

    if (showRepoInfos(dir_path) < 0) {
        return -1;
    }

    printf("\n\nPlease enter the ID of the repository: ");
    scanf("%d", &repoId);

    problems = getProblems(dir_path, repoId);
    if (problems == NULL) {
        return -1;
    }

    problemCount = cJSON_GetArraySize(problems);
    printf("\n\nInformation of problems: \n");
    for (int i=0; i<problemCount; i++) {
        cJSON *item = cJSON_GetArrayItem(problems, i);
        cJSON *problemId = cJSON_GetObjectItem(item, "id");
        cJSON *problemTitle = cJSON_GetObjectItem(item, "title");
        cJSON *problemText = cJSON_GetObjectItem(item, "text");
        cJSON *problemUUID = cJSON_GetObjectItem(item, "uuid");

        printf("------------------------------\n");
        printf("Problem no.%d\n", i+1);
        printf("id: %d\n", problemId->valueint);
        printf("title: %s\n", problemTitle->valuestring);
        printf("text: %s\n", problemText->valuestring);
        printf("file link: %s\n", problemUUID->valuestring);
        printf("------------------------------\n\n");
    }
    return 0;
}

int testProblem() {
    return 0;
}

int submitResult(char home[], char location[]) {
    const char* repo_path = "../myRepos";
    char* output = (char*) malloc(128*sizeof(char));
    int repoId = 0, problemId = 0, testcaseId = 0, problemCount = 0, testCaseCount = 0;
    cJSON *problems = NULL;
    cJSON *testcases = NULL;
    cJSON *testcase = NULL;

    if (showRepoInfos(repo_path) < 0) {
        return -1;
    }

    printf("\n\nPlease enter the ID of the repository: ");
    scanf("%d", &repoId);

    problems = getProblems(repo_path, repoId);
    if (problems == NULL) {
        return -1;
    }

    problemCount = cJSON_GetArraySize(problems);
    if (problemCount == 0) {
        fprintf(stderr, "There are no problems.\n");
        return -1;
    }

    printf("\n\nInformation of problems: \n");
    for (int i=0; i<problemCount; i++) {
        cJSON *item = cJSON_GetArrayItem(problems, i);
        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *title = cJSON_GetObjectItem(item, "title");
        cJSON *text = cJSON_GetObjectItem(item, "text");
        cJSON *uuid = cJSON_GetObjectItem(item, "uuid");

        printf("------------------------------\n");
        printf("Problem no.%d\n", i+1);
        printf("id: %d\n", id->valueint);
        printf("title: %s\n", title->valuestring);
        printf("text: %s\n", text->valuestring);
        printf("file link: %s\n", uuid->valuestring);
        printf("------------------------------\n\n");
    }

    printf("\n\nPlease enter the ID of the problem: ");
    scanf("%d", &problemId);

    // testcases = getTestCases(repo_path, repoId, problemId);
    testcases = getTestCases(problems, problemId);
    if (testcases == NULL) {
        return -1;
    }

    testCaseCount = cJSON_GetArraySize(testcases);
    if (testCaseCount == 0) {
        fprintf(stderr, "There are no testcase.\n");
        return -1;
    }

    printf("\n\nInformation of testcase: \n");
    for (int i=0; i<testCaseCount; i++) {
        cJSON *item = cJSON_GetArrayItem(testcases, i);
        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *input = cJSON_GetObjectItem(item, "input");
        cJSON *output = cJSON_GetObjectItem(item, "output");
        cJSON *isHidden = cJSON_GetObjectItem(item, "isHidden");

        printf("------------------------------\n");
        printf("TestCase no.%d\n", i+1);
        printf("id: %d\n", id->valueint);
        printf("input: %s\n", input->valuestring);
        printf("output: %s\n", output->valuestring);
        printf("isHidden: %s\n", isHidden->valuestring);
        printf("------------------------------\n\n");
    }

    printf("\n\nPlease enter the ID of the testcase: ");
    scanf("%d", &testcaseId);

    testcase = getTestCase(testcases, testcaseId);
    if (testcase == NULL) {
        fprintf(stderr, "Please check the testcaseID\n");
        return -1;
    }
    
    cJSON *input = cJSON_GetObjectItem(testcase, "input");

    execute(location, input->valuestring, output);
    // output 을 SHA256 암호화해야함.
    // 나중에 한번에 하기

    if (submitResultHTTP(home, output, testcaseId) < 0) {
        return -1;
    }
    return 0;
}

int showRepoInfos(char* dir_path) {
    DIR* dir;
    struct dirent *entry;
    
    dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "Fail to open directory.\n");
        return -1;
    }

    printf("\n\nInformation of your repositories: \n");
    printf("%-20s %-20s\n", "repositoryId", "repositoryName");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char* file_name = entry->d_name;
            size_t file_name_len = strlen(file_name);

            if (file_name_len > 5 && strcmp(file_name + file_name_len - 5, ".json") == 0) {
                char file_path[256];
                snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, file_name);

                FILE* file = fopen(file_path, "r");
                if (!file) {
                    return -1;
                }

                fseek(file, 0, SEEK_END);
                long file_size = ftell(file);
                fseek(file, 0, SEEK_SET);

                char* json_data = (char*) malloc(file_size + 1);
                if (!json_data) {
                    fclose(file);
                    return -1;
                }

                fread(json_data, 1, file_size, file);
                fclose(file);
                json_data[file_size] = '\0';

                cJSON* root = cJSON_Parse(json_data);
                if (!root) {
                    free(json_data);
                    return -1;
                }
                cJSON* repoId = cJSON_GetObjectItem(root, "id");
                cJSON* repoName = cJSON_GetObjectItem(root, "name");

                if (repoId != NULL && repoName != NULL) {
                    printf("%-20d %-20s\n", repoId->valueint, repoName->valuestring);
                } else {
                    fprintf(stderr, "It seems like JSON file is corrupted. Please fetch the repository information again.\n");
                    return -1;
                }
            }
        }
    }

    return 0;
}

cJSON* getProblems(char* dir_path, int repoId) {
    DIR* dir;
    struct dirent *entry;
    cJSON* problems = NULL;
    char numeric_part[20];
    char strRepoId[20];
    snprintf(strRepoId, sizeof(strRepoId), "%d", repoId);
    
    dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "Fail to open directory.\n");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char* file_name = entry->d_name;
            size_t file_name_len = strlen(file_name);

            if (file_name_len > 5 && strcmp(file_name + file_name_len - 5, ".json") == 0) {
                int i = 0;
                while (file_name[i] != '\0' && file_name[i] != '_') {
                    if (isdigit(file_name[i])) {
                        numeric_part[i] = file_name[i];
                        i++;
                    } else {
                        break;
                    }
                }
                numeric_part[i] = '\0';

                if (strcmp(numeric_part, strRepoId) == 0) {
                    char file_path[256];
                    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, file_name);

                    FILE* file = fopen(file_path, "r");
                    if (!file) {
                        return NULL;
                    }

                    fseek(file, 0, SEEK_END);
                    long file_size = ftell(file);
                    fseek(file, 0, SEEK_SET);

                    char* json_data = (char*) malloc(file_size + 1);
                    if (!json_data) {
                        fclose(file);
                        return NULL;
                    }

                    fread(json_data, 1, file_size, file);
                    fclose(file);
                    json_data[file_size] = '\0';

                    cJSON* root = cJSON_Parse(json_data);
                    if (!root) {
                        free(json_data);
                        return NULL;
                    }

                    problems = cJSON_GetObjectItem(root, "Problem");
                } else {
                    continue;
                }
            }
        }
    }

    return problems;
}

cJSON* getTestCases(cJSON* problems, int problemId) {
    int size = cJSON_GetArraySize(problems);

    for (int i=0; i<size; i++) {
        cJSON *item = cJSON_GetArrayItem(problems, i);
        cJSON *id = cJSON_GetObjectItem(item, "id");

        if (problemId == id->valueint) {
            return cJSON_GetObjectItem(item, "testCase");
        }
    }
    return NULL;
} 

cJSON* getTestCase(cJSON* testcases, int testcaseId) {
    int size = cJSON_GetArraySize(testcases);

    for (int i=0; i<size; i++) {
        cJSON *item = cJSON_GetArrayItem(testcases, i);
        cJSON *id = cJSON_GetObjectItem(item, "id");

        if (testcaseId == id->valueint) {
            return item;
        }
    }

    return NULL;
}
