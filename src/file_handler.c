#include <stdio.h>
#include <string.h>
#include "file_handler.h"

#define STUDENT_FILE  "data/students.dat"
#define HISTORY_FILE  "data/history.dat"
#define SKILLS_FILE   "data/skills.dat"

/* Save one Student record (append) */
void saveStudent(Student *s) {
    FILE *f = fopen(STUDENT_FILE, "ab");
    if (f == NULL) {
        printf("Error: Cannot open %s\n", STUDENT_FILE);
        return;
    }
    fwrite(s, sizeof(Student), 1, f);
    fclose(f);
}

/* Load all Student records into array */
int loadAllStudents(Student arr[], int *n) {
    FILE *f = fopen(STUDENT_FILE, "rb");
    *n = 0;
    if (f == NULL) return 0; /* file not yet created */

    while (fread(&arr[*n], sizeof(Student), 1, f) == 1)
        (*n)++;

    fclose(f);
    return *n;
}

/* Rewrite entire students.dat with updated array */
void updateStudentFile(Student arr[], int n) {
    FILE *f = fopen(STUDENT_FILE, "wb");
    if (f == NULL) {
        printf("Error: Cannot rewrite %s\n", STUDENT_FILE);
        return;
    }
    fwrite(arr, sizeof(Student), n, f);
    fclose(f);
}

/* Save one PredictionRecord (append) */
void savePrediction(PredictionRecord *pr) {
    FILE *f = fopen(HISTORY_FILE, "ab");
    if (f == NULL) {
        printf("Error: Cannot open %s\n", HISTORY_FILE);
        return;
    }
    fwrite(pr, sizeof(PredictionRecord), 1, f);
    fclose(f);
}

/* Load prediction history for a specific student */
int loadHistory(int studentRef, PredictionRecord arr[], int *n) {
    FILE *f = fopen(HISTORY_FILE, "rb");
    *n = 0;
    if (f == NULL) return 0;

    PredictionRecord temp;
    while (fread(&temp, sizeof(PredictionRecord), 1, f) == 1) {
        if (temp.studentRef == studentRef) {
            arr[(*n)++] = temp;
        }
    }
    fclose(f);
    return *n;
}



/* Save (or update) a SkillProfile for a student.
   Reads all existing profiles, replaces the one matching
   studentRef (or appends if not found), then rewrites the file. */
void saveSkillProfile(SkillProfile *sp) {
    SkillProfile profiles[100];
    int n = 0, i, found = 0;

    FILE *f = fopen(SKILLS_FILE, "rb");
    if (f != NULL) {
        while (fread(&profiles[n], sizeof(SkillProfile), 1, f) == 1)
            n++;
        fclose(f);
    }

    for (i = 0; i < n; i++) {
        if (profiles[i].studentRef == sp->studentRef) {
            profiles[i] = *sp;
            found = 1;
            break;
        }
    }
    if (!found) {
        if (n < 100) profiles[n++] = *sp;
    }

    f = fopen(SKILLS_FILE, "wb");
    if (f == NULL) {
        printf("Error: Cannot write %s\n", SKILLS_FILE);
        return;
    }
    fwrite(profiles, sizeof(SkillProfile), n, f);
    fclose(f);
}

/* Load the SkillProfile for a student.
   Returns 1 if found, 0 if not. */
int loadSkillProfile(int studentRef, SkillProfile *sp) {
    FILE *f = fopen(SKILLS_FILE, "rb");
    if (f == NULL) return 0;

    SkillProfile temp;
    while (fread(&temp, sizeof(SkillProfile), 1, f) == 1) {
        if (temp.studentRef == studentRef) {
            *sp = temp;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
