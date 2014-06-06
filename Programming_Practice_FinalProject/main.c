/*
141020 김영하

기말과제 
*/

#include "main.h"
#include "list.h"

int main(void)
{
	char buf[256];	
	int lineCount = 0;
	int fileOpenResult = 0;
	FILE *fileRead = NULL, *fileWrite = NULL;
	StudentList studentList;
	studentList.count = 0; studentList.firstStudent = NULL; studentList.lastStudent = NULL;  // student list 초기화

	fileOpenResult = openStudentFile(&fileRead, &fileWrite);
	if (fileOpenResult == -1) return -1;	// data.txt 읽어오기, 없으면 만들기, 만들기 실패하면 종료하기
	
	if (fileOpenResult == 0)
	{
		while (fgets(buf, sizeof(buf), fileRead) != NULL)	// file 읽어서 구조체 할당하고 list에 추가하기
		{
			if (lineCount++ == 0) continue;	// 첫줄 무시		
			Student *s = parseStudentStr(buf);
			addStudentToList(s, &studentList);
		}
	}
	if (fileRead) fclose(fileRead);
	if (fileWrite) fclose(fileWrite);

	// main program loop
	int choice = 0;
	int changeExists = 0;
	while (choice != 9)
	{
		runMainMenu(&choice);
		switch (choice)
		{
			case 1:		printStudentList(&studentList);		
						break;	

			case 2:		addNewStudent(&studentList); 
						changeExists++;	
						break;

			case 3:		modifyStudentInfo(&studentList);
						changeExists++;
						break;

			case 4:		deleteStudentInfo(&studentList);
						changeExists++;
						break;

			case 5:		saveListToFile(&studentList); 
						changeExists = 0;	
						break;

			case 9:		if (onQuit(changeExists)) saveListToFile(&studentList);
						break;

			default:	printf("Invalid choice!\n");		
						break;
		}
	}
		
	cleanupOnExit(&studentList, fileRead, fileWrite);
	return 0;
}

//
// void return functions	/////////////////
//
int onQuit(int changeExists)
{
	char ans = 'y';
	if (changeExists) {
		printf("\n\n\tChanges exist! Do you want to save before quitting? (Y/N)");
		scanf("%c", &ans);

		if (ans == 'Y' || ans == 'y')
		{
			return 1;
		}
	}
	
	return 0;
}

void runMainMenu(int *choice)
{
	printf("\n\n----- STUDENT INFO MANAGEMENT PROGRAM -----\n\n");
	printf("\t-MAIN MENU-\n\n");
	printf("\tWhat do you want to do?\n");
	printf("\t1. View student info\n");
	printf("\t2. Add new student info\n");
	printf("\t3. Modify student info\n");
	printf("\t4. Delete student info\n");
	printf("\t5. Save student info to file\n");	
	printf("\t9. Quit\n");
	printf("\n\tYour choice?>");
	scanf("%d", choice);
	printf("\n\n");
	while (getchar() != '\n');
}


//
// integer return functions	/////////////////
//

int openFileRead(const char* fileName, FILE **fp)
{
	fopen_s(fp, fileName, "r");
	if (*fp == NULL) {
		printf("Error in file read open for %s\n", fileName);
		return -1;
	}
	return 0;
}
int openFileWrite(const char* fileName, FILE **fp)
{
	fopen_s(fp, fileName, "w");
	if (*fp == NULL) {
		printf("Error in file write open for %s\n", fileName);
		return -1;
	}
	return 0;
}
int openStudentFile(FILE **fileRead, FILE **fileWrite)
{
	if (openFileRead("data.txt", fileRead) == -1)
	{
		// if open file has error, then create the file
		if (openFileWrite("data.txt", fileWrite) == -1)
		{
			printf("Cannot create data.txt! Serious error! \n");
			return -1;
		}
		else
		{
			printf("Created empty data.txt, because file did not exist\n");
			fprintf(*fileWrite, "%s\t%s\t%s\t%s\t\n", "회원ID", "회원이름", "회원주소", "핸드폰번호");
			fflush(*fileWrite);

			return 1;
		}
	}

	return 0;
}

//
// pointer return functions	/////////////////
//
