#include "../includes/common.h" 
#include "../includes/cJSON.h"
/**
 * related with problem module
*/

static int show(int argc, char*argv[]) {
    return 0;
}

static int get(int argc, char*argv[]) {
    return 0;
}

static int submit(int argc, char*argv[]) {
    return 0;
}

static int test(int argc, char*argv[]) {
    FILE *testcase_file;                        
    char* json_data = NULL;                     // 파일 내용 저장할 변수
    char file_name[64] = "../testcase/a.json";  // 파일 위치
    long file_size;
    size_t result;

    // 파일 열기  
    testcase_file = fopen(file_name, "rb");

    if (testcase_file == NULL) {
        fprintf(stderr, "ERROR\n");
        return 0;
    } 

    // 파일 크기 계산 
    fseek(testcase_file, 0, SEEK_END);
    file_size = ftell(testcase_file);
    fseek(testcase_file, 0, SEEK_SET);

    // 파일 크기만큼 메모리 할당
    json_data = (char*) malloc(file_size+1);
    if (json_data == NULL) {
        fprintf(stderr, "fail to malloc");
        fclose(testcase_file);
        return 0;
    }

    // 파일 전체 내용 읽어오기 
    result = fread(json_data, 1, file_size, testcase_file);
    json_data[result] = '\0';

    cJSON* root = cJSON_Parse(json_data);

    cJSON* input = cJSON_GetObjectItem(root, "input");
    cJSON* output = cJSON_GetObjectItem(root, "output");

    if (input->valuestring) {
        printf("input: %s\n", input->valuestring);
    } else if (input->valueint) {
        printf("input: %d\n", input->valueint);
    } else if (input->valuedouble) {
        printf("input: %f\n", input->valuedouble);
    }

    free(json_data);
    fclose(testcase_file);
}

int problem(int argc, char*argv[]) {
    char command[CMDSIZE];
	if(argc<3 || !strcpy(command,argv[2])){
		fprintf(stderr,"no command ...\n");
		problemInfo();
		exit(-1);
	}

    if(!strncmp(command,"show",4)) {
        // TODO
        printf("show!\n");
        if (show(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "get", 3)) {
        // TODO
        printf("get!\n");
        if (get(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "submit", 6)) {
        // TODO
        printf("submit!\n");
        if (submit(argc, argv)) {
            fprintf(stderr, "ERROR\n");
            exit(-1);
        }
    } else if (!strncmp(command, "test", 4)) {
        // TODO
        printf("test!\n");
        test(argc, argv);
    }
}