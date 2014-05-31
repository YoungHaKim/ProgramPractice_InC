#ifndef LIST_H
#define LIST_H

#include "final.h"


typedef struct student_ {
	int id;
	char name[20];
	char address[40];
	char phoneNum[20];
	struct student_ *nextStudentPtr;
} Student;


typedef struct studentList_ {
	Student *firstStudent;
	Student *lastStudent;
	int count;
} StudentList;

void addStudentToList(Student *s, StudentList *list);
void cleanupOnExit(StudentList *list, FILE *readFile, FILE *writeFile);
void printStudentList(StudentList *list);
void saveListToFile(StudentList *s);
void addNewStudent(StudentList *s);

Student* parseStudentStr(char *str);

#endif