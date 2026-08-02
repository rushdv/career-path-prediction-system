#include <stdio.h>
#include "gap_analysis.h"
#include "colors.h"

void analyzeGap(SkillProfile *sp, CareerPath *cp) {
    float skills[6];
    const char *skillNames[6] = {
        "Programming",
        "Networking",
        "Design",
        "Analytics",
        "Communication",
        "Security"
    };
      skills[0] = sp->programming;
    skills[1] = sp->networking;
    skills[2] = sp->design;
    skills[3] = sp->analytics;
    skills[4] = sp->communication;
    skills[5] = sp->security;

    CLEAR_SCREEN();
    printf("\n");
     /* Dynamic title */
    char title[60];
    int tlen = snprintf(title, sizeof(title), "  GAP ANALYSIS  \xe2\x80\x94  %s  ", cp->name);
    printBanner(title, tlen - 6); /* subtract hidden UTF-8 bytes in em-dash */
    printf("\n");

    /* Column header */
    printf("  " C_HEADER BOLD "%-16s  %7s  %9s  %6s  %-22s\n" RESET,
           "Skill", "Yours", "Required", "Gap", "Status");
    printf("  " C_DIM);
    int d; for (d = 0; d < 66; d++) printf(BOX_H);
    printf(RESET "\n\n");
