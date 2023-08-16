#include "info.h"

void basicInfo(void)
{
	printf("Hello! Welcome to Edged Judger Platform for Student (EJP-S)\n");
	printf("You can use command below: \n");
	printf("1. repo\n\n");
	printf("2. problem\n\n");
	printf("3. testcase\n\n");
}

void repoInfo(void)
{
	basicInfo();
	printf("repo command : \n\n");
	printf("1. show: It shows the repositories you can access. This command requires \"h\" option that means host address. \n\n");
	printf("2. get: It fetches all repositories information that you can access. This command requires \"h\" option that means host address. \n\n");
	printf(":) \n\n");
}

void workbookInfo(void)
{
	basicInfo();
	printf("workbook command : \n");
}

void problemInfo(void)
{
	basicInfo();
	printf("problem command : \n\n ");
	printf("1. get: Receive problems written by your instructor.\n\n ");
	printf("2. show: Read all problems written by your instructor. \n\n ");
	printf("3. submit: Submit your answer(maybe..?) code.\n\n ");
	printf("4. test: Test your code with testcase published by your instructor. \n\n");
	printf(":) \n\n");
}

void testcaseInfo(void)
{
	basicInfo();
	printf("testcase command : \n\n");
	printf("1. get: Receive testcases published by your instructor.\n\n ");
	printf(":) \n\n");
}