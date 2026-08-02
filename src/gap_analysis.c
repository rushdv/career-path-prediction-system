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
