/*
141020 김영하

기말과제
*/

#include "list.h"

g_largestID = 0;

void addStudentToList(Student *s, StudentList *list)
{
	s->nextStudentPtr = NULL;

	if (s->id > g_largestID)
		g_largestID = s->id;

	if (list->count == 0)
	{
		list->count++;
		list->firstStudent = s;
		list->lastStudent = s;
	}
	else
	{
		list->lastStudent->nextStudentPtr = s;
		list->lastStudent = s;
		list->count++;
	}
}
void deleteStudentFromList(Student *s, StudentList *list)
{
	Student *sPtr;
	Student *prevPtr;

	if (s == NULL || list == NULL) return;
	if (list->count == 0) return;

	sPtr = list->firstStudent;

	if (s == sPtr) // item is the first in the list
	{
		list->firstStudent = list->firstStudent->nextStudentPtr;
		free(s);

		printf("\n\tDelete succesful!\n");
		return;
	}

	// more than 1 items exists, so traverse list
	prevPtr = sPtr;
	sPtr = sPtr->nextStudentPtr;

	while (sPtr != NULL)  // keep going until the next pointer equals s
	{	
		if (sPtr == s) // found it!
		{
			prevPtr->nextStudentPtr = sPtr->nextStudentPtr;

			if (sPtr->nextStudentPtr == NULL) // sPtr is the last item in the list
				list->lastStudent = prevPtr;

			free(s);

			printf("\n\tDelete succesful!\n");
			break;
		}

		prevPtr = sPtr;
		sPtr = sPtr->nextStudentPtr;
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

void printStudentInfo(Student *sPtr)
{
	printf("%7s %10s %30s %20s\n\n", "ID", "NAME", "ADDRESS", "PHONENO");
	printf("%7d %10s %30s %20s\n", sPtr->id, sPtr->name, sPtr->address, sPtr->phoneNum);
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
	int largestId = g_largestID;
	char nameInput[20];
	char phoneInput[25];	
	char addressInput[40];

	if (s == NULL) {
		printf("\n Memory Allocation Error in creating new student!\n\n");
		return;
	}
		
	largestId++;
	s->id = largestId;
	printf("\n\tID\t\t:%d\n", s->id);
	
	getNameInput(nameInput);	
	strcpy(s->name, nameInput);
	
	getPhoneInput(phoneInput);
	strcpy(s->phoneNum, phoneInput);

	getAddressInput(addressInput);
	strcpy(s->address, addressInput);
	
	addStudentToList(s, list);

	printf("\n\tSuccessfully added new student to list!\n");
	printStudentInfo(s);
}

void getNameInput(char *name)
{
	fflush(stdin);
	char buf[256];

	do {
		printf("\tEnter Name\t:");
		gets(buf);
		
		if (strlen(buf) > 20)
		{
			printf("Sorry, name input cannot be longer than 20 characters\n");  // long name의 예: Sergey Aleynikov 도 20글자 이내임
		}
	} while (strlen(buf) > 20);

	for (int i = 0; i < 20; i++)
	{
		name[i] = buf[i];
		if (buf[i] == '\0') break;
	}
}
void getPhoneInput(char *phoneInput)
{
	
	char bug[256];
	int inputError = 0;

	do {
		fflush(stdin);
		inputError = 0;

		printf("\tEnter PhoneNum\t:");
		gets(bug);

		int upperBound = strlen(bug);
		char space = ' ';
		char zero = '0';
		char nine = '9';
		char plus = '+';
		char dash = '-';
		char parenLeft = '(';
		char parenRight = ')';
		
		if (strlen(bug) > 25) // 최대한 긴 번호, 국제전화의 경우도 25글자 이내임  +62 (2) 43 5674 2339
		{
			printf("Error in input! phone number cannot be longer than 25 chars!\n");
			inputError = 1;

			continue;
		}

		for (int i = 0; i < (int)strlen(bug); i++)
		{
			if (bug[i] == '\0') break;

			if (bug[i] < zero || bug[i] > nine) {
				if (bug[i] != plus && bug[i] != dash
					&& bug[i] != parenLeft && bug[i] != parenRight
					&& bug[i] != space) {

					printf("Error in input! you typed %c in phone num\n", bug[i]);

					inputError = 1;
					break;
				}
			}
		}

	} while (inputError == 1);

	for (int i = 0; i < 25; i++)
	{
		phoneInput[i] = bug[i];
		if (phoneInput[i] == '\0') break;
	}
}
void getAddressInput(char *address)
{
	fflush(stdin);
	char buf[256];

	do {
		printf("\tEnter address\t:");
		gets(buf);

		if (strlen(buf) > 39)
		{
			printf("Sorry, address input cannot be longer than 40 characters\n");  // long name의 예: Sergey Aleynikov 도 20글자 이내임
			
			continue;
		}
	} while (strlen(buf) > 39);

	for (int i = 0; i < 40; i++)
	{
		address[i] = buf[i];
		if (buf[i] == '\0') break;
	}
}

void modifyStudentInfo(StudentList *list)
{
	int choice = 0;
	int id;
	char name[30];
	char phoneNum[25];
	char address[40];
	Student *s;
	s = NULL;

	do{
		choice = findStudent(list, &s);

		if (choice == 0) return;

		if (s == NULL)
		{
			printf("\n\tError! The student you are looking for does not exist!\n");
			continue;
		}

		printf("\n\tSearch Success! Result:\n");
		printStudentInfo(s);

		do
		{
			printf("\n\tWhich data field do you want to modify?");
			printf("\n\t1.ID\n");
			printf("\t2.Name\n");
			printf("\t3.Phone Number\n");
			printf("\t4.Address\n");
			printf("\n\t0. End Edit\n");
			printf("\n\tYour choice?:");
			scanf(" %d", &choice);

			fflush(stdin);

			switch (choice)
			{
			case 0: return;

			case 1:
				printf("\n\tInput new ID:");
				scanf("%d", &id);
				s->id = id;
				printf("\n\t ID has been modified to %d\n", id);
				break;

			case 2:
				printf("\n\tInput new Name:");
				getNameInput(name);
				strcpy(s->name, name);
				printf("\n\t name has been modified to %s\n", name);
				break;

			case 3:
				printf("\n\tInput new PhoneNum:");
				getPhoneInput(phoneNum);
				strcpy(s->phoneNum, phoneNum);
				printf("\n\t phoneNum has been modified to %s\n", phoneNum);
				break;

			case 4:
				printf("\n\tInput new Address:");
				gets(address);
				strcpy(s->address, address);
				printf("\n\t address has been modified to %s\n", address);
				break;

			default:
				printf("\n\tInvalid choice:\n");
				continue;			
			}

			printf("\n\tCurrent Record is now:\n");
			printStudentInfo(s);
						

		} while (choice != 0);

		printf("\n\tEdit Done!\n");
		

	} while (choice != 0);


}

void deleteStudentInfo(StudentList *list)
{
	Student *s;
	s = NULL;
	int choice = 0;
	char deleteChoice;

	do
	{
		choice = findStudent(list, &s);
		
		if (choice == 0) return;

		if (s == NULL)
		{
			printf("\n\tError! Could not find student to delete!\n");
			continue;
		}

		printf("\n\tSearch Success! Result:\n");
		printStudentInfo(s);

		printf("\n\tPress y to delete!\n");
		fflush(stdin);
		scanf("%c", &deleteChoice);

		if (deleteChoice == 'y' || deleteChoice == 'Y');
		{
			deleteStudentFromList(s, list);
		}

	} while (choice != 0);
}


int findStudent(StudentList *list, Student **s)
{
	int choice = 0;

	int id;
	char name[30];
	char phoneNum[25];
	
	do
	{
		printf("\n\tEnter the type of info to search for:\n");
		printf("\n\t1.ID\n");
		printf("\t2.Name\n");
		printf("\t3.Phone Number\n");
		printf("\n\t0. Cancel Search\n");
		printf("\n\tYour choice?:");
		scanf(" %d", &choice);

		fflush(stdin);

		switch (choice)
		{
		case 1:
			printf("\n\tEnter id:");
			scanf("%d", &id);
			*s = getStudentByID(list, id);
			return choice;

		case 2:
			printf("\n\tEnter name:");
			gets(name);
			*s = getStudentByName(list, name);
			return choice;

		case 3:
			printf("\n\tEnter name:");
			gets(phoneNum);
			*s = getStudentByPhone(list, phoneNum);
			return choice;

		case 0: break;

		default:
			break;
		}

	} while (choice != 0);

	return choice;
}

Student* getStudentByID(StudentList *list, int id)
{
	Student *s;
	s = list->firstStudent;

	while (s != NULL)
	{
		if (s->id == id)
			return s;

		s = s->nextStudentPtr;
	}

	return (Student*)NULL;
}
Student* getStudentByName(StudentList *list, char *name)
{
	Student *s;
	s = list->firstStudent;

	while (s != NULL)
	{
		if (strcmp(s->name, name) == 0)
			return s;

		s = s->nextStudentPtr;
	}

	return (Student*)NULL;
}
Student* getStudentByPhone(StudentList *list, char *phone)
{
	Student *s;
	s = list->firstStudent;

	while (s != NULL)
	{
		if (strcmp(s->phoneNum, phone) == 0)
			return s;

		s = s->nextStudentPtr;
	}

	return (Student*)NULL;
}

