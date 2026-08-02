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

    /* Skill bars as ASCII art in file */
    float skillVals[6] = {
        sp->programming, sp->networking, sp->design,
        sp->analytics, sp->communication, sp->security
    };
    const char *skillLabels[6] = {
        "Programming", "Networking", "Design",
        "Analytics", "Communication", "Security"
    };
    int i, b;
    for (i = 0; i < 6; i++) {
        int filled = (int)((skillVals[i] / 10.0f) * 20);
        int empty  = 20 - filled;
        fprintf(f, " %-16s [", skillLabels[i]);
        for (b = 0; b < filled; b++) fprintf(f, "#");
        for (b = 0; b < empty;  b++) fprintf(f, ".");
        fprintf(f, "] %.1f/10\n", skillVals[i]);
    }

    fprintf(f, "\n------------------------------------------------\n");
    fprintf(f, " CAREER PATH RANKINGS\n");
    fprintf(f, "------------------------------------------------\n");
    for (i = 0; i < NUM_CAREERS; i++) {
        int filled = (int)((scores[i] / 10.0f) * 20);
        int empty  = 20 - filled;
        fprintf(f, " %d. %-24s [", i + 1, ranked[i].name);
        for (b = 0; b < filled; b++) fprintf(f, "#");
        for (b = 0; b < empty;  b++) fprintf(f, ".");
        fprintf(f, "] %.2f/10\n", scores[i]);
    }

    fprintf(f, "\n TOP RECOMMENDATION : %s (%.2f/10)\n\n", ranked[0].name, scores[0]);
    fprintf(f, "================================================\n");
    fprintf(f, " END OF REPORT\n");
    fprintf(f, "================================================\n");

    fclose(f);