#include <stdio.h>
#include "assessment.h"
#include "colors.h"
#include "input_handler.h"

/* Collect one skill rating with inline bar preview */
static float getSkillRating(const char *skillName) {
    inputPrompt(skillName);
    float val = getFloatInput(0.0f, 10.0f);
    /* Show instant bar feedback */
    printf("  " C_DIM "           ");
    printSkillBar("", val, 20);
    printf(RESET);
    return val;
}

/* ── Run skill assessment ───────────────────────────────── */
SkillProfile runAssessment(int studentRef) {
    SkillProfile sp;
    sp.studentRef = studentRef;

    CLEAR_SCREEN();
    printf("\n");
    printBanner("  SKILL ASSESSMENT  ", 20);
    printf("\n");

    printf("  " C_INFO "Rate each skill on a scale of " RESET
           C_VALUE BOLD "0" RESET C_DIM " (no knowledge) " RESET
           C_INFO "to " RESET C_VALUE BOLD "10" RESET C_DIM " (expert)" RESET "\n\n");

    /* Dividers between skill groups */
    printf("  " C_DIM "─── Technical Skills " RESET
           C_DIM "──────────────────────────────────\n\n" RESET);

    sp.programming = getSkillRating("Programming  (C/Python/Java)");
    sp.networking  = getSkillRating("Networking   (TCP/Routing)  ");
    sp.security    = getSkillRating("Security     (Crypto/PenTest)");

    printf("\n  " C_DIM "─── Analytical & Creative Skills " RESET
           C_DIM "──────────────────────\n\n" RESET);

    sp.analytics      = getSkillRating("Data Analytics (SQL/Stats)  ");
    sp.design         = getSkillRating("Design         (UI/UX/GFX)  ");
    sp.communication  = getSkillRating("Communication  (Writing/PPT)");

    /* Summary */
    printf("\n");
    printBanner("  ASSESSMENT SUMMARY  ", 22);
    printf("\n");

    printSkillBar("Programming",   sp.programming,  20);
    printSkillBar("Networking",    sp.networking,   20);
    printSkillBar("Security",      sp.security,     20);
    printSkillBar("Analytics",     sp.analytics,    20);
    printSkillBar("Design",        sp.design,       20);
    printSkillBar("Communication", sp.communication,20);

    printf("\n");
    printSuccess("Assessment recorded successfully!");
    pauseScreen();

    return sp;
}
