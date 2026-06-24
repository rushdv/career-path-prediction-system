#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int   id;
    char  name[50];
    char  studentID[20];
    float cgpa;
    char  department[30];
    int   isActive;      /* 1 = active, 0 = deleted */
} Student;

/* Function declarations */
void createStudent(void);
void viewStudent(const char *studentID);
void updateStudent(const char *studentID);
void deleteStudent(const char *studentID);
void listAllStudents(void);

#endif