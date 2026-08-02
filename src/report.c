#include <stdio.h>
#include <string.h>
#include <time.h>
#include "report.h"
#include "colors.h"

void generateReport(Student *s, SkillProfile *sp,
                    CareerPath ranked[], float scores[]) {

    char filename[100];
    snprintf(filename, sizeof(filename), "reports/%s_report.txt", s->studentID);

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printError("Could not create report file. Check that 'reports/' folder exists.");
        return;
    }

     time_t now = time(NULL);
    char timeStr[30];
    struct tm *t = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

    /* ── Write plain-text report (no ANSI — file viewer compatible) ── */
    fprintf(f, "================================================\n");
    fprintf(f, "    CAREER PATH PREDICTION SYSTEM REPORT\n");
    fprintf(f, "    Northern University Bangladesh\n");
    fprintf(f, "================================================\n");
    fprintf(f, " Generated  : %s\n\n", timeStr);

    fprintf(f, "------------------------------------------------\n");
    fprintf(f, " STUDENT INFORMATION\n");
    fprintf(f, "------------------------------------------------\n");
    fprintf(f, " Name       : %s\n",   s->name);
    fprintf(f, " Student ID : %s\n",   s->studentID);
    fprintf(f, " CGPA       : %.2f\n", s->cgpa);
    fprintf(f, " Department : %s\n\n", s->department);

    fprintf(f, "------------------------------------------------\n");
    fprintf(f, " SKILL ASSESSMENT\n");
    fprintf(f, "------------------------------------------------\n");
