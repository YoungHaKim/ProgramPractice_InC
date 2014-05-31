/*
141020 김영하

기말과제
*/

#include "list.h"

void addStudentToList(Student *s, StudentList *list)
{
	s->nextStudentPtr = NULL;

	if (list->count == 0)
	{
		list->count++;
		list->firstStudent = s;
		list->lastStudent = s;
	}
	else
	{
		Student *sPtr;
		sPtr = list->firstStudent;
		while (sPtr->nextStudentPtr != NULL)
		{
			sPtr = sPtr->nextStudentPtr;
		}

		sPtr->nextStudentPtr = s;

		list->lastStudent = s;
		list->count++;
	}
}
void cleanupOnExit(StudentList *list, FILE *readFile, FILE *writeFile)
{
	//if (list) free(list);
	if (readFile) fclose(readFile);
	if (writeFile) fclose(writeFile);
}
void printStudentList(StudentList *list)
{
	printf("%7s %10s %30s %20s\n\n", "ID", "NAME", "ADDRESS", "PHONENO");

	int count = 1;
	int page = 1;
	Student *sPtr;
	sPtr = list->firstStudent;
	while (sPtr != NULL)
	{
		if (count++ % 10 == 0)
		{
			printf("\n... page %d, press Enter to view more\n\n", page++);
			getchar();

			printf("%7s %10s %30s %20s\n\n", "ID", "NAME", "ADDRESS", "PHONENO");
		}

		printf("%7d %10s %30s %20s\n", sPtr->id, sPtr->name, sPtr->address, sPtr->phoneNum);
		sPtr = sPtr->nextStudentPtr;
	}

	printf("\n\nEND\n\n");
}


void saveListToFile(StudentList *list)
{
	FILE *fileWrite;
	Student *ptr;
	
	if (openFileWrite("data.txt", &fileWrite) == -1) return;
	if (fileWrite == NULL)
	{
		printf("\nError in opening file to write\n");
		return;
	}

	ptr = list->firstStudent;

	fprintf(fileWrite, "%s\t%s\t%s\t%s\t\n", "회원ID", "회원이름", "회원주소", "핸드폰번호");
	while (ptr != NULL)
	{
		fprintf(fileWrite, "%d\t%s\t%s\t%s\n", ptr->id, ptr->name, ptr->address, ptr->phoneNum);
		ptr = ptr->nextStudentPtr;
	}

	fflush(fileWrite);
	fclose(fileWrite);

	printf("Save complete!\n\n");
}


Student* parseStudentStr(char *str)
{
	// allocate and initialize values
	Student *s = (Student*)malloc(sizeof(Student));
	s->id = 0;
	strcpy(s->name, "empty");
	strcpy(s->address, "empty");
	strcpy(s->phoneNum, "empty");
	s->nextStudentPtr = NULL;


	//remove newline character from data string
	int len = strlen(str);
	if (str[len - 1] == '\n')
	{
		str[len - 1] = '\0';
	}

	// tokenize data
	char delimitStr[] = "\t";
	char *token;
	token = strtok(str, delimitStr);
	s->id = atoi(token);

	token = strtok(NULL, delimitStr);
	strcpy(s->name, token);

	token = strtok(NULL, delimitStr);
	strcpy(s->address, token);

	token = strtok(NULL, delimitStr);
	strcpy(s->phoneNum, token);

	//printf("Added %d %s %s %s\n", s->id, s->name, s->address, s->phoneNum);

	// return student pointer
	return s;
}

void addNewStudent(StudentList *list)
{
	Student *ptr = list->firstStudent;
	Student *s = (Student*)malloc(sizeof(Student));
	int largestId = ptr->id;	

	if (s == NULL) {
		printf("\n Memory Allocation Error in creating new student!\n\n");
		return;
	}
	
	while (ptr != NULL)
	{
		if (ptr->id > largestId)
			largestId = ptr->id;

		ptr = ptr->nextStudentPtr;
	}
	
	largestId++;
	s->id = largestId;
	printf("\n\tID\t\t:%d\n", s->id);
	printf("\tEnter Name\t:");
	gets(s->name);
	printf("\tEnter Address\t:");
	gets(s->address);
	printf("\tEnter PhoneNum\t:");
	gets(s->phoneNum);

	addStudentToList(s, list);

}