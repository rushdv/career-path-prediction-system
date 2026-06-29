#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "student.h"
#include "prediction.h"

void saveStudent(Student *s);
int  loadAllStudents(Student arr[], int *n);
void updateStudentFile(Student arr[], int n);
void savePrediction(PredictionRecord *pr);
int  loadHistory(int studentRef, PredictionRecord arr[], int *n);
void searchByName(const char *name);

#endif