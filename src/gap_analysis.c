
/* ── Rank and display top careers ───────────────────────── */
void rankCareers(const SkillProfile *sp) {
    CareerPath careers[NUM_CAREERS];
    int count;
    getCareers(careers, &count);

    float scores[NUM_CAREERS];
    for (int i = 0; i < count; i++) {
        scores[i] = calculateScore(sp, &careers[i]);
    }

    /* Bubble sort to rank careers (descending) */
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (scores[j] < scores[j + 1]) {
                float tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                CareerPath tempPath = careers[j];
                careers[j] = careers[j + 1];
                careers[j + 1] = tempPath;
            }
        }
    }

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

    pauseScreen();
}