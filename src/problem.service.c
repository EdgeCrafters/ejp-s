#include "http.h"

void printProblemInfo(cJSON* problems, int index);
void printTestcaseInfo(cJSON* testcases, int index);
cJSON* getProblems(char* dir_path, int repoId);
cJSON* getTestCases(cJSON* problems, int problemId);
cJSON* getTestCase(cJSON* testcases, int testcaseId);

int getProblemsFile(const char home[], const char location[]) {
    char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
    int repoId = 0;
    cJSON* problems = NULL;

    if (showRepoInfos(repo_path) < 0) {
        return -1;
    }

    fprintf(stderr,"\n\nPlease enter the ID of the repository: ");
    scanf("%d", &repoId);

    problems = getProblems(repo_path, repoId);
    if (problems == NULL) {
        return -1;
    }

    int i = 0, problemCount = cJSON_GetArraySize(problems);
    cJSON *problem = NULL;
    fprintf(stderr,"\n\nInformation of problems: \n");
    fprintf(stderr,"%-20s %-20s %-20s\n", "Id", "Title", "Description");
    for (problem = cJSON_GetArrayItem(problems,i); i<problemCount && problem; problem = cJSON_GetArrayItem(problems,i++)) 
    {
        cJSON *problemId = cJSON_GetObjectItem(problem,"id"), *title = cJSON_GetObjectItem(problem,"title"), *text = cJSON_GetObjectItem(problem,"text");
        if(!problemId || !title || !text)
            continue;
        
        fprintf(stderr,"%-20d %-20s %-20s\n",problemId->valueint,title->valuestring,text->valuestring);
    }

    int problemId;
    fprintf(stderr,"select problem id to download: ");
    scanf("%d",&problemId);

    getProblemFilesHTTP(home,problemId,location);

    return 0;
}

int showProblems() {
    char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
    int repoId = 0, problemCount = 0;
    cJSON* problems = NULL;

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
    printf("\n\nInformation of problems: \n");
    for (int i=0; i<problemCount; i++) {
        printProblemInfo(problems, i);
    }
    return 0;
}

int testProblem(char location[]) {
    char* user_output = (char*) malloc(512*sizeof(char));
    int testcaseId = 0;
    cJSON *testcases = NULL;
    cJSON *testcase = NULL;

    if (showTestcases(&testcases) < 0) {
        return -1;
    }

    printf("\n\nPlease enter the ID of the testcase: ");
    scanf("%d", &testcaseId);

    testcase = getTestCase(testcases, testcaseId);
    cJSON* testcaseType = cJSON_GetObjectItem(testcase, "isHidden");
    if (strcmp("HIDDEN", testcaseType->valuestring) == 0) {
        fprintf(stderr, "You can not test with hidden testcase.\n");
        return -1;
    }
    if (testcase == NULL) {
        fprintf(stderr, "Please check the testcaseID\n");
        return -1;
    }

    cJSON *input = cJSON_GetObjectItem(testcase, "input");
    cJSON *output = cJSON_GetObjectItem(testcase, "output");

    execute(location, input->valuestring, user_output);

    size_t output_length = strlen(user_output);
    while (output_length > 0 && 
        (user_output[output_length - 1] == ' ' 
        || user_output[output_length - 1] == '\t' 
        || user_output[output_length - 1] == '\n' 
        || user_output[output_length - 1] == '\r'
    )) {
        output_length--;
    }

    sprintf(user_output, "%.*s", output_length, user_output);
    user_output = SHA256(user_output);

    if (!strcmp(user_output, output->valuestring)) {
        printf("PASS\n");
    } else {
        printf("FAILED\n");
    }

    free(user_output);
    return 0;
}

int submitResult(char home[], char location[]) {
    char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
    char* output = (char*) malloc(512*sizeof(char));
    int testcaseId = 0;
    cJSON *testcases = NULL;
    cJSON *testcase = NULL;

    if (showTestcases(&testcases) < 0) {
        return -1;
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
    fprintf(stderr,"%s",output);

    if (submitResultHTTP(home, output, testcaseId) < 0) {
        return -1;
    }
    free(output);
    return 0;
}

int showTestcases(cJSON** testcasesPtr) {
    char repo_path[512];
	sprintf(repo_path, "%s/../myRepos", exe);
    int repoId = 0, problemId = 0, problemCount = 0, testCaseCount = 0;
    cJSON *problems = NULL;
    cJSON *testcases = NULL;

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
        printProblemInfo(problems, i);
    }

    printf("\n\nPlease enter the ID of the problem: ");
    scanf("%d", &problemId);

    testcases = getTestCases(problems, problemId);
    if (testcases == NULL) {
        return -1;
    }
    if (testcasesPtr != NULL) {
        *testcasesPtr = testcases;
    }

    testCaseCount = cJSON_GetArraySize(testcases);
    if (testCaseCount == 0) {
        fprintf(stderr, "There are no testcase.\n");
        return -1;
    }

    printf("\n\nInformation of testcase: \n");
    for (int i=0; i<testCaseCount; i++) {
        printTestcaseInfo(testcases, i);
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

    fprintf(stderr,"\n\nInformation of your repositories: \n");
    fprintf(stderr,"%-20s %-20s\n", "repositoryId", "repositoryName");
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

void printProblemInfo(cJSON* problems, int index) {
    cJSON *item = cJSON_GetArrayItem(problems, index);
    cJSON *id = cJSON_GetObjectItem(item, "id");
    cJSON *title = cJSON_GetObjectItem(item, "title");
    cJSON *text = cJSON_GetObjectItem(item, "text");
    cJSON *uuid = cJSON_GetObjectItem(item, "uuid");

    printf("------------------------------\n");
    printf("%-11s%d\n", "Problem no.", index+1);
    printf("------------------------------\n");
    printf("%-15s%d\n", "id: ", id->valueint);
    printf("%-15s%s\n", "title: ", title->valuestring);
    printf("%-15s%s\n", "text: : ", text->valuestring);
    printf("%-15s%s\n", "file link: ", uuid->valuestring);
    printf("------------------------------\n\n");
}

void printTestcaseInfo(cJSON* testcases, int index) {
    cJSON *item = cJSON_GetArrayItem(testcases, index);
    cJSON *id = cJSON_GetObjectItem(item, "id");
    cJSON *input = cJSON_GetObjectItem(item, "input");
    cJSON *output = cJSON_GetObjectItem(item, "output");
    cJSON *isHidden = cJSON_GetObjectItem(item, "isHidden");

    printf("------------------------------\n");
    printf("%-12s%d\n", "TestCase no.", index+1);
    printf("------------------------------\n");
    printf("%-15s%d\n", "id: ", id->valueint);
    printf("%-15s%s\n", "input: ", input->valuestring);
    printf("%-15s%s\n", "output: ", output->valuestring);
    printf("%-15s%s\n", "isHidden: ", isHidden->valuestring);
    printf("------------------------------\n\n");
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
