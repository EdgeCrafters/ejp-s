#include "info.h"

void basicInfo(void)
{
	printf("Hello! Welcome to Edged Judger Platform for Student (EJP-S)\n");
	printf("You can use command below: \n");
	printf(" 1. problem\n\n");
	printf(" 2. repo\n\n");
}

void repoInfo(void)
{
	basicInfo();
	printf("repo command : \n");
	printf(" 1. show: This command shows repositories you can access. \n    This command requires \"-h\" option that means host address.\n\n");
	printf(" 2. get: This command fetches all repositories information that you can access. \n    This command requires \"-h\" option that means host address.\n\n");
	printf(" 3. clean: This command delete all repositories information. \n    It is recommended to use before using \"get\" command.\n\n");
}

void problemInfo(void)
{
	basicInfo();
	printf("problem command : \n\n");
	printf(" 1. show: Read all questions written by your instructor. \n\n");
	printf(" 2. submit: Submit your answer(maybe..?) code.\n    This command requires \"-h\" option that means host address and \"-l\" option that means location of your answer file(compiled file).\n\n");
	printf(" 3. test: Test your code with testcase published by your instructor.\n    This command requires \"-l\" option that means location of your answer file(compiled file).\n\n");
	printf(" 4. testcase: Read all testcases.\n\n");
}