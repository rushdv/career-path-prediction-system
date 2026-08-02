#include <stdio.h>
#include <string.h>
#include <time.h>
#include "prediction.h"
#include "colors.h"
#include "file_handler.h"
/* Career path definitions
   Skill order: [programming, networking, design, analytics, communication, security] */
static CareerPath careers[NUM_CAREERS] = {
    {
        "Software Developer",
        {0.35f, 0.10f, 0.15f, 0.20f, 0.10f, 0.10f},
        {6.0f,  3.0f,  3.0f,  4.0f,  3.0f,  2.0f}
    },
      {
        "Network Engineer",
        {0.15f, 0.40f, 0.05f, 0.15f, 0.10f, 0.15f},
        {3.0f,  7.0f,  2.0f,  3.0f,  3.0f,  4.0f}
    },
    {
        "UI/UX Designer",
        {0.15f, 0.05f, 0.40f, 0.15f, 0.20f, 0.05f},
        {3.0f,  1.0f,  7.0f,  3.0f,  5.0f,  1.0f}
    },
    {
        "Data Analyst",
        {0.20f, 0.05f, 0.10f, 0.40f, 0.15f, 0.10f},
        {4.0f,  2.0f,  2.0f,  7.0f,  4.0f,  2.0f}
    },
    {
        "IT Project Manager",
        {0.15f, 0.10f, 0.10f, 0.20f, 0.35f, 0.10f},
        {3.0f,  3.0f,  3.0f,  4.0f,  7.0f,  2.0f}
    },
     {
        "Cybersecurity Analyst",
        {0.20f, 0.20f, 0.05f, 0.15f, 0.10f, 0.30f},
        {4.0f,  5.0f,  1.0f,  3.0f,  3.0f,  7.0f}
    }
};

/* Rank medal for top 3 */
static const char *rankLabel(int rank) {
    if (rank == 1) return SYM_STAR "  " C_RANK1 BOLD "#1";
    if (rank == 2) return "   " C_RANK2 BOLD "#2";
    if (rank == 3) return "   " C_RANK3      "#3";
    return "      ";
}

/* ── Calculate weighted score ───────────────────────────── */
float calculateScore(SkillProfile *sp, CareerPath *cp) {
    float skills[6];
    skills[0] = sp->programming;
    skills[1] = sp->networking;
    skills[2] = sp->design;
    skills[3] = sp->analytics;
    skills[4] = sp->communication;
    skills[5] = sp->security;

    float score = 0.0f;
    int i;
    for (i = 0; i < 6; i++) {
        score += skills[i] * cp->weights[i];
    }
    return score;
}
/* ── Rank and display careers ───────────────────────────── */
void rankCareers(SkillProfile *sp) {
    float     scores[NUM_CAREERS];
    CareerPath ranked[NUM_CAREERS];
    int i, j;

    for (i = 0; i < NUM_CAREERS; i++) {
        ranked[i] = careers[i];
        scores[i] = calculateScore(sp, &careers[i]);
    }

    /* Bubble sort descending */
    for (i = 0; i < NUM_CAREERS - 1; i++) {
        for (j = 0; j < NUM_CAREERS - 1 - i; j++) {
            if (scores[j] < scores[j + 1]) {
                float      ts = scores[j];  scores[j]  = scores[j+1];  scores[j+1]  = ts;
                CareerPath tc = ranked[j];  ranked[j]  = ranked[j+1];  ranked[j+1]  = tc;
            }
        }
    }

    CLEAR_SCREEN();
    printf("\n");
    printBanner("  CAREER PATH PREDICTION RESULTS  ", 34);
    printf("\n");

    for (i = 0; i < NUM_CAREERS; i++) {
        /* Row color */
        if      (i == 0) printf(C_RANK1);
        else if (i == 1) printf(C_RANK2);
        else if (i == 2) printf(C_RANK3);
        else             printf(C_DIM);

        printf("  %s" RESET "  " C_VALUE "%-24s" RESET "  ", rankLabel(i+1), ranked[i].name);
        printScoreBar(scores[i], 16);
        printf("  " BOLD "%.2f/10" RESET "\n", scores[i]);

        if (i == 0) {
            /* Divider after top pick */
            printf("  " C_DIM);
            int d; for (d = 0; d < 48; d++) printf(BOX_H);
            printf(RESET "\n");
        }
    }
  printf("\n");

    /* Top recommendation highlight box */
    printf(C_BORDER "  " BOX_TL);
    int b; for (b = 0; b < 48; b++) printf(BOX_H);
    printf(BOX_TR RESET "\n");

    printf(C_BORDER "  " BOX_V RESET "  "
           C_ACCENT BOLD SYM_ARROW "  Top Recommendation: " RESET
           C_VALUE BOLD "%-24s" RESET "  " C_BORDER BOX_V RESET "\n", ranked[0].name);

    printf(C_BORDER "  " BOX_V RESET "  "
           C_DIM    "   Weighted Score:    " RESET
           C_SUCCESS BOLD "%-6.2f / 10.00" RESET
           "              " C_BORDER BOX_V RESET "\n", scores[0]);

    printf(C_BORDER "  " BOX_BL);
    for (b = 0; b < 48; b++) printf(BOX_H);
    printf(BOX_BR RESET "\n\n");
