#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "student.h"
#include "prediction.h"
#include "assessment.h"

void saveStudent(Student *s);
int  loadAllStudents(Student arr[], int *n);
void updateStudentFile(Student arr[], int n);
void savePrediction(PredictionRecord *pr);
int  loadHistory(int studentRef, PredictionRecord arr[], int *n);

/* Skill profile persistence */
void saveSkillProfile(SkillProfile *sp);
int  loadSkillProfile(int studentRef, SkillProfile *sp);

/* Export to CSV */
void exportStudentsToCSV(void);

#endif