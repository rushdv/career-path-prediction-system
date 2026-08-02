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

    