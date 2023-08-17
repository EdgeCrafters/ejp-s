#include "../includes/common.h" 
#include "../includes/cJSON.h"
#include "../includes/test.h"
// #include "../includes/http.h"
/**
 * related with problem module
*/

// 필요한 옵션
// h: host 주소
static int show(int argc, char*argv[]) {
    char* user_host = (char*) malloc(128 * sizeof(char));
    if (argc == 5 && strcmp(argv[3], "h") == 0) {
        user_host = argv[4];
        userLogin(user_host);
        showAllProblems(user_host);
    } else {
        printf("Please see the command description typing \"%s %s\"\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }
    return 1;
}

static int get(int argc, char*argv[]) {
    return 0;
}

static int submit(int argc, char*argv[]) {
    int bias = 0;               // db 에서 가져온 bias 를 저장할 변수
    int repoId = 0;             // repo 정보
    int hiddenCaseId = 0;       // hiddenCase 정보

    // 우선 로그인 먼저
    // 사용자에게 host 입력받기
    char* user_host = (char*) malloc(128 * sizeof(char));
    printf("Input your host address: ");
    scanf("%s", user_host);
    userLogin(user_host);

    // 나중에 구현 필요
    repoId = getRepoId();

    // 나중에 구현 필요
    hiddenCaseId = getHiddenCaseId();

    // 나중에 구현 필요
    bias = getBias(repoId, hiddenCaseId);

    // 사용자에게 output 위치 받기
    char* user_output = (char*) malloc(128 * sizeof(char));
    printf("Input your location of output file: ");
    scanf("%s", user_output);

    printf("repoId: %d\nhiddenCaseId: %d\nbias: %d\nlocation of output: %s\n", repoId, hiddenCaseId, bias, user_output);

    // cookie 정보 확인하기
    getSessionInfo();
    printf("\n\n");

    // 실제 요청을 보내야 하는 곳
    if (postResult(repoId, hiddenCaseId)) {
        printf("The request was sent successfully!\n");
    } else {
        printf("Failed to request...\n");
    }

    free(user_output);
    free(user_host);
}

static int test(int argc, char*argv[]) {
    FILE *testcase_file;                        
    char* json_data = NULL;                     // 파일 내용 저장할 변수
    char file_name[64] = "../testcase/a.json";  // 파일 위치
    char* user_file = (char*) malloc(128 * sizeof(char));
    char* user_output = (char*) malloc(128 * sizeof(char));
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

    // 사용자가 테스트해보려는 파일 정보 알아내기
    printf("Input your location of compiled C file: ");
    scanf("%s", user_file);
    printf("%s\n", user_file);

    execute(user_file, input->valuestring, user_output);

    if (!strcmp(user_output, output->valuestring)) {
        printf("PASS!\n");
    } else {
        printf("FAILED...\n");
    }

    printf("Your output is: %s\n", user_output);
    printf("Expected output is: %s\n", output->valuestring);

    cJSON_Delete(root);
    free(json_data);
    free(user_output);
    free(user_file);
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
        show(argc, argv);
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
        submit(argc, argv);
    } else if (!strncmp(command, "test", 4)) {
        // TODO
        printf("test!\n");
        test(argc, argv);
    }
}