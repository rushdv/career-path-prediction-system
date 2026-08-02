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