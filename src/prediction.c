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
