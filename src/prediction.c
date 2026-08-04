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

        /* Each row: "  " rank(6) "  " name(%-22s) "  " bar(14) "  " score
           rank label display widths:
             rank 1: SYM_STAR(1)+"  "+C_RANK1+BOLD+"#1" = 1+2+2 = 5 display
             rank 2: "   "+"#2"                          = 3+2   = 5 display
             rank 3: "   "+"#3"                          = 3+2   = 5 display
             others: "      "                            = 6     = 6 display
           Use fixed 6-char display slot for rank label. */
        printf("  %s" RESET "  " C_VALUE "%-22s" RESET "  ",
               rankLabel(i + 1), ranked[i].name);
        printScoreBar(scores[i], 14);
        printf("  " BOLD "%.2f/10" RESET "\n", scores[i]);

        /* Extra blank line between items so bars don't touch */
        if (i < NUM_CAREERS - 1 && i != 0)
            printf("\n");

        if (i == 0) {
            /* Divider after #1 */
            printf("  " C_DIM);
            int d; for (d = 0; d < 58; d++) printf(BOX_H);
            printf(RESET "\n\n");
        }
    }
    printf("\n");

    /* ── Recommendation box ─────────────────────────────────
       Uses boxRowRaw() so padding is always exact.
       Row 1: "▶  Top Recommendation : <name>"
              SYM_ARROW = ▶ (1 display, 3 bytes)
              "  Top Recommendation : " = 23 chars
              name = up to ~22 chars
       Row 2: "   Weighted Score     : X.XX / 10.00" = 36 chars
    ── */
    {
        char plain1[80], colored1[256];
        /* snprintf counts bytes; ▶ = 3 bytes but 1 display char → subtract 2 */
        int blen1 = snprintf(plain1, sizeof(plain1),
                             "\xe2\x96\xb6  Top Recommendation : %s", ranked[0].name);
        int dlen1 = blen1 - 2; /* adjust for ▶ multi-byte */
        snprintf(colored1, sizeof(colored1),
                 C_ACCENT BOLD SYM_ARROW RESET
                 C_VALUE "  Top Recommendation : " RESET
                 C_VALUE BOLD "%s" RESET, ranked[0].name);

        char plain2[48], colored2[128];
        int dlen2 = snprintf(plain2, sizeof(plain2),
                             "   Weighted Score     : %.2f / 10.00", scores[0]);
        snprintf(colored2, sizeof(colored2),
                 C_DIM "   Weighted Score     : " RESET
                 C_SUCCESS BOLD "%.2f / 10.00" RESET, scores[0]);

        boxTop();
        boxRowRaw(colored1, dlen1);
        boxRowRaw(colored2, dlen2);
        boxBottom();
    }
    printf("\n");

    /* Save to history */
    PredictionRecord pr;
    pr.studentRef = sp->studentRef;
    pr.score      = scores[0];
    strncpy(pr.topCareer, ranked[0].name, sizeof(pr.topCareer) - 1);
    pr.topCareer[sizeof(pr.topCareer) - 1] = '\0';

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(pr.timestamp, sizeof(pr.timestamp), "%Y-%m-%d %H:%M:%S", t);
    savePrediction(&pr);

    pauseScreen();
}

/* ── Expose careers for other modules ───────────────────── */
void getCareers(CareerPath out[], int *count) {
    int i;
    for (i = 0; i < NUM_CAREERS; i++) out[i] = careers[i];
    *count = NUM_CAREERS;
}