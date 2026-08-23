#include <stdio.h>
#include <string.h>
#include "gap_analysis.h"
#include "colors.h"
#include "assessment.h"

/* ══════════════════════════════════════════════════════════
   GAP BAR
   Shows user's score vs target on a 0-10 scale.
   - Filled portion up to min(current, target): green if OK, red if below
   - If current > target: extra filled portion in cyan (excess)
   - Target marker ▼ shown above the bar at the correct position
   Width = number of bar cells (each cell = 1.0 point on 10.0 scale)
   ══════════════════════════════════════════════════════════ */
static void printGapBar(float current, float required, int width) {
    /* cell i represents the range (i * 10/width) to ((i+1) * 10/width) */
    int userCell = (int)((current  / 10.0f) * (float)width);
    int reqCell  = (int)((required / 10.0f) * (float)width);
    if (userCell > width) userCell = width;
    if (reqCell  > width) reqCell  = width;
    if (userCell < 0)     userCell = 0;
    if (reqCell  < 0)     reqCell  = 0;

    int ok = (current >= required);

    /* ── top marker line ─────────────────────────────────
       Print a ▼ at the target position, spaces elsewhere   */
    printf("  ");   /* align with bar bracket */
    for (int i = 0; i < width; i++) {
        if (i == reqCell) {
            printf("%s%s%s", CC(C_WARNING), CC(BOLD), "v");  /* target marker */
        } else {
            printf(" ");
        }
    }
    printf("\n");

    /* ── bar ─────────────────────────────────────────────
       [  filled  |  gap / excess  ]                        */
    fputs(UI_PAD "  ", stdout);
    printf("%s[%s", CC(C_BORDER2), CC(RESET));

    for (int i = 0; i < width; i++) {
        if (ok) {
            /* user meets or exceeds target */
            if (i < reqCell) {
                /* met requirement zone — green */
                printf("%s", CC(C_SUCCESS));
                fputs(SYM_BAR_F, stdout);
            } else if (i < userCell) {
                /* excess zone — cyan */
                printf("%s", CC(C_ACCENT));
                fputs(SYM_BAR_F, stdout);
            } else {
                printf("%s", CC(C_DIM));
                fputs(SYM_BAR_E, stdout);
            }
        } else {
            /* user below target */
            if (i < userCell) {
                /* what user has — orange/red */
                if (current >= 6.0f)      printf("%s", CC(C_WARNING));
                else if (current >= 3.5f) printf("%s", "\033[38;5;208m");
                else                      printf("%s", CC(C_ERROR));
                fputs(SYM_BAR_F, stdout);
            } else if (i < reqCell) {
                /* gap zone — dim red */
                printf("%s", CC(C_ERROR));
                fputs(SYM_BAR_E, stdout);
            } else {
                printf("%s", CC(C_DIM));
                fputs(SYM_BAR_E, stdout);
            }
        }
    }
    printf("%s]%s", CC(C_BORDER2), CC(RESET));
}

/* ══════════════════════════════════════════════════════════
   MAIN GAP ANALYSIS SCREEN
   ══════════════════════════════════════════════════════════ */
void analyzeGap(const SkillProfile *sp, const CareerPath *cp) {
    CLEAR_SCREEN();
    printf("\n");
    printBanner("SKILL GAP ANALYSIS", 18);
    printf("\n");

    printf(UI_PAD "%sAnalysing against:%s %s%s%s%s\n\n",
           CC(C_DIM), CC(RESET), CC(C_ACCENT), CC(BOLD), cp->name, CC(RESET));

    /* ── Legend ──────────────────────────────────────────── */
    printf(UI_PAD "  %s%s%s Met  ", CC(C_SUCCESS), SYM_BAR_F, CC(RESET));
    printf("%s%s%s Excess  ", CC(C_ACCENT), SYM_BAR_F, CC(RESET));
    printf("%s%s%s Gap  ", CC(C_ERROR), SYM_BAR_E, CC(RESET));
    printf("%s%sv%s Target\n\n", CC(C_WARNING), CC(BOLD), CC(RESET));

    const char *skills[] = {
        "Programming", "Networking", "Design",
        "Analytics",   "Communication", "Security"
    };
    int levels[] = {
        sp->level_programming, sp->level_networking, sp->level_design,
        sp->level_analytics,   sp->level_communication, sp->level_security
    };
    float current[] = {
        sp->programming, sp->networking, sp->design,
        sp->analytics,   sp->communication, sp->security
    };
    float required[] = {
        cp->req_programming, cp->req_networking, cp->req_design,
        cp->req_analytics,   cp->req_communication, cp->req_security
    };

    int gaps_found = 0;

    for (int i = 0; i < 6; i++) {
        int ok = (current[i] >= required[i]);
        float gap = required[i] - current[i];

        /* ── Skill name + your rating ─────────────────── */
        char levelStr[32];
        snprintf(levelStr, sizeof(levelStr), "%s (%.1f)", levelName(levels[i]), current[i]);

        printf(UI_PAD "%s%-15s%s  %s%-18s%s  target: %s%.1f%s\n",
               ok ? CC(C_SUCCESS) : CC(C_INFO),
               skills[i], CC(RESET),
               CC(C_VALUE), levelStr, CC(RESET),
               CC(C_WARNING), required[i], CC(RESET));

        /* ── Gap bar (with target marker on top) ─────── */
        printGapBar(current[i], required[i], 20);

        /* ── Status badge ─────────────────────────────── */
        printf("  ");
        if (ok) {
            printf("%s%s[  OK  ]%s\n\n", CC(C_SUCCESS), CC(BOLD), CC(RESET));
        } else {
            printf("%s%s[ NEED +%.1f ]%s\n\n", CC(C_ERROR), CC(BOLD), gap, CC(RESET));
            gaps_found++;
        }
    }

    printHRule(BOX_WIDTH + 4);
    printf("\n");

    /* ── Result / Action Plan ─────────────────────────── */
    if (gaps_found == 0) {
        printSuccess("You meet or exceed all requirements for this career path!");
    } else {
        printSectionHeader("Action Plan & Recommended Resources");

        const char *courses[] = {
            "Coursera: Python for Everybody  |  LeetCode DSA Practice",
            "Cisco CCNA Guide  |  Network+ Certification",
            "CalArts UI/UX Specialization  |  Figma Mastery Course",
            "Google Data Analytics Certificate  |  SQL for Data Science",
            "LinkedIn Learning: Technical Communication",
            "CompTIA Security+  |  TryHackMe Cyber Defense Path"
        };

        for (int i = 0; i < 6; i++) {
            if (current[i] < required[i]) {
                float g = required[i] - current[i];
                printf(UI_PAD "%s%s%s  %s%-15s%s  "
                       "yours: %s%.1f%s  target: %s%.1f%s  "
                       "%sneeds +%.1f pts%s\n",
                       CC(C_ERROR), SYM_CROSS, CC(RESET),
                       CC(C_INFO), skills[i], CC(RESET),
                       CC(C_VALUE), current[i], CC(RESET),
                       CC(C_ACCENT), required[i], CC(RESET),
                       CC(C_WARNING), g, CC(RESET));
                printf(UI_PAD "    %s%s%s  %s%s%s\n\n",
                       CC(C_PRIMARY), SYM_ARROW, CC(RESET),
                       CC(C_MUTED), courses[i], CC(RESET));
            }
        }
    }

<<<<<<< HEAD
=======
    CLEAR_SCREEN();
    printf("\n");
    printBanner("CAREER PREDICTION RESULTS", 25);
    printf("\n");

    printf(UI_PAD "%sBased on your skill profile, here are your top career matches:%s\n\n",
           C_DIM, RESET);

    const char *rankColors[] = { C_RANK1,    C_RANK2,    C_RANK3   };
    const char *rankStars[]  = { SYM_RANK1,  SYM_RANK2,  SYM_RANK3 };
    const char *rankTags[]   = { "BEST MATCH",  "2nd Match",  "3rd Match" };

    for (int i = 0; i < 3 && i < count; i++) {
        float pct = scores[i] * 10.0f;

        /* Rank header line */
        printf(UI_PAD "%s%s%s  %s#%d%s  %s%s%s\n",
               CC(rankColors[i]), rankStars[i], CC(RESET),
               CC(BOLD), i + 1, CC(RESET),
               CC(C_DIM), rankTags[i], CC(RESET));

        /* Career name + score bar + percentage */
        printf(UI_PAD "  %s%s%-34s%s  ",
               CC(rankColors[i]), CC(BOLD), careers[i].name, CC(RESET));
        printScoreBar(scores[i], 14);
        printf("  %s%s%.0f%%%s\n\n",
               CC(rankColors[i]), CC(BOLD), pct, CC(RESET));
    }

    printHRule(BOX_WIDTH + 4);

    /* Save best result to history */
    PredictionRecord pr;
    pr.studentRef = sp->studentRef;
    strncpy(pr.topCareer, careers[0].name, sizeof(pr.topCareer) - 1);
    pr.topCareer[sizeof(pr.topCareer) - 1] = '\0';
    pr.score = scores[0];
    db_save_prediction(&pr);

>>>>>>> dd9cd7e011177b3dd54bb3ef6a3d23b092b0be1d
    pauseScreen();
}
