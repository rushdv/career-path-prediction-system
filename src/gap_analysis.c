#include <stdio.h>
#include "gap_analysis.h"
#include "colors.h"

void analyzeGap(SkillProfile *sp, CareerPath *cp) {
    const char *skillNames[6] = {
        "Programming",
        "Networking",
        "Design",
        "Analytics",
        "Communication",
        "Security"
    };

    float skills[6];
    skills[0] = sp->programming;
    skills[1] = sp->networking;
    skills[2] = sp->design;
    skills[3] = sp->analytics;
    skills[4] = sp->communication;
    skills[5] = sp->security;

    CLEAR_SCREEN();
    printf("\n");

    /* Dynamic banner title — cp->name is ASCII so byte len = display len */
    char title[60];
    int tlen = snprintf(title, sizeof(title), "  GAP ANALYSIS  \xe2\x80\x94  %s  ", cp->name);
    /* \xe2\x80\x94 is em-dash: 3 bytes, 1 display char → subtract 2 hidden bytes */
    printBanner(title, tlen - 2);
    printf("\n");

    /* ── Column header ──────────────────────────────────── */
    printf("  " C_HEADER BOLD "%-16s  %7s  %9s  %6s  %-22s\n" RESET,
           "Skill", "Yours", "Required", "Gap", "Status");
    printf("  " C_DIM);
    { int d; for (d = 0; d < 68; d++) printf(BOX_H); }
    printf(RESET "\n\n");

    /* ── Per-skill rows ─────────────────────────────────── */
    int i;
    int hasGap = 0;

    for (i = 0; i < 6; i++) {
        float gap = cp->minRequired[i] - skills[i];

        /* Skill name */
        printf("  " C_INFO "%-16s" RESET "  ", skillNames[i]);

        /* Your score — colour-coded */
        if      (skills[i] >= 7.0f) printf(C_SUCCESS BOLD);
        else if (skills[i] >= 4.0f) printf(C_WARNING BOLD);
        else                         printf(C_ERROR   BOLD);
        printf("%5.1f" RESET "    ", skills[i]);

        /* Required score */
        printf(C_VALUE "%7.1f" RESET "  ", cp->minRequired[i]);

        /* Gap + status */
        if (gap > 0) {
            printf(C_ERROR   BOLD "%+5.1f" RESET "  ", -gap);
            printf(C_NEEDS   BOLD "  " SYM_WARN "  NEEDS IMPROVEMENT" RESET "\n");
            hasGap = 1;
        } else {
            printf(C_SUCCESS BOLD "%+5.1f" RESET "  ", -gap);
            printf(C_OK      BOLD "  " SYM_CHECK "  OK" RESET "\n");
        }

        /* Mini progress bars */
        printf("  " C_DIM "                 Your: " RESET);
        printSkillBar("", skills[i], 16);

        printf("  " C_DIM "                 Need: " RESET);
        printSkillBar("", cp->minRequired[i], 16);
        printf("\n");
    }

    printf("  " C_DIM);
    { int d; for (d = 0; d < 68; d++) printf(BOX_H); }
    printf(RESET "\n\n");

    /* ── Summary box ────────────────────────────────────── */
    if (!hasGap) {
        /* "✔  You meet ALL requirements for <name>"
           SYM_CHECK = ✔ (1 display, 3 bytes) → subtract 2 from snprintf byte count */
        char plain[64], colored[256];
        int blen = snprintf(plain, sizeof(plain),
                            "\xe2\x9c\x94  You meet ALL requirements for %s", cp->name);
        int dlen = blen - 2; /* UTF-8 adjustment for ✔ */
        snprintf(colored, sizeof(colored),
                 C_SUCCESS BOLD SYM_CHECK
                 "  You meet ALL requirements for " RESET
                 C_VALUE BOLD "%s" RESET, cp->name);

        boxTop();
        boxRowRaw(colored, dlen);
        boxBottom();
        printf("\n");
    } else {
        printf("  " C_WARNING SYM_WARN BOLD
               "  Focus on the skills marked "
               C_NEEDS "[NEEDS IMPROVEMENT]" RESET
               C_WARNING " to close your gaps." RESET "\n\n");
    }

    pauseScreen();
}
