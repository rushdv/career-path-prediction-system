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
       int i;
    int hasGap = 0;

    for (i = 0; i < 6; i++) {
        float gap = cp->minRequired[i] - skills[i];

        /* Skill name + your bar */
        printf("  " C_INFO "%-16s" RESET "  ", skillNames[i]);

        /* Your score colored */
        if      (skills[i] >= 7.0f) printf(C_SUCCESS BOLD);
        else if (skills[i] >= 4.0f) printf(C_WARNING BOLD);
        else                         printf(C_ERROR   BOLD);
        printf("%5.1f" RESET "    ", skills[i]);
           /* Required score */
        printf(C_VALUE "%7.1f" RESET "  ", cp->minRequired[i]);

        if (gap > 0) {
            /* Gap: show deficit in red */
            printf(C_ERROR BOLD "%+5.1f" RESET "  ", -gap);
            printf(C_NEEDS BOLD "  \xe2\x9a\xa0  NEEDS IMPROVEMENT" RESET "\n");
            hasGap = 1;
        } else {
            printf(C_SUCCESS BOLD "%+5.1f" RESET "  ", -gap);
            printf(C_OK     BOLD "  \xe2\x9c\x94  OK" RESET "\n");
        }

        /* Mini progress bar for this skill */
        printf("  " C_DIM "                 Your: " RESET);
        printSkillBar("", skills[i], 16);

        printf("  " C_DIM "                 Need: " RESET);
        printSkillBar("", cp->minRequired[i], 16);
        printf("\n");
    }
