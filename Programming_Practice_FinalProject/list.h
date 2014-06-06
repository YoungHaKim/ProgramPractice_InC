#ifndef LIST_H
#define LIST_H

#include "final.h"


typedef struct student_ {
	int id;
	char name[20];
	char address[40];
	char phoneNum[25];
	struct student_ *nextStudentPtr;
} Student;


typedef struct studentList_ {
	Student *firstStudent;
	Student *lastStudent;
	int count;
} StudentList;

void addStudentToList(Student *s, StudentList *list);
void deleteStudentFromList(Student *s, StudentList *list);
void cleanupOnExit(StudentList *list, FILE *readFile, FILE *writeFile);
void modifyStudentInfo(StudentList *list);
void deleteStudentInfo(StudentList *list);
void printStudentList(StudentList *list);
void printStudentInfo(Student *sPtr);
void saveListToFile(StudentList *s);
void addNewStudent(StudentList *s);

void getNameInput(char *name);
void getPhoneInput(char *phone);

int findStudent(StudentList *list, Student **s);

Student* getStudentByID(StudentList *list, int id);
Student* getStudentByName(StudentList *list, char *name);
Student* getStudentByPhone(StudentList *list, char *phone);
Student* parseStudentStr(char *str);


#endif