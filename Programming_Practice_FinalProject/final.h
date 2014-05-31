#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FINAL_H
#define FINAL_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int openFileRead(const char* fileName, FILE **fp);
int openFileWrite(const char* fileName, FILE **fp);
int openStudentFile(FILE **fileRead, FILE **fileWrite);

int onQuit(int changeExists);
void runMainMenu(int* choice);

#endif