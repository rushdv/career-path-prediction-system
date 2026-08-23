#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/prediction.h"
#include "include/assessment.h"

int main() {
    SkillProfile sp;
    memset(&sp, 0, sizeof(sp));
    sp.programming = 2.0;
    sp.networking = 2.0;
    sp.design = 2.0;
    sp.analytics = 2.0;
    sp.communication = 2.0;
    sp.security = 2.0;

    CareerPath cp;
    memset(&cp, 0, sizeof(cp));
    strcpy(cp.name, "Software Engineer");
    cp.req_programming = 8.0;
    cp.req_networking = 3.0;
    cp.req_design = 4.0;
    cp.req_analytics = 5.0;
    cp.req_communication = 6.0;
    cp.req_security = 4.0;

    float diff = 0.0f;
    diff += (sp.programming >= cp.req_programming)   ? 0 : (cp.req_programming - sp.programming);
    diff += (sp.networking >= cp.req_networking)     ? 0 : (cp.req_networking - sp.networking);
    diff += (sp.design >= cp.req_design)             ? 0 : (cp.req_design - sp.design);
    diff += (sp.analytics >= cp.req_analytics)       ? 0 : (cp.req_analytics - sp.analytics);
    diff += (sp.communication >= cp.req_communication) ? 0 : (cp.req_communication - sp.communication);
    diff += (sp.security >= cp.req_security)         ? 0 : (cp.req_security - sp.security);

    float max_possible = 60.0f;
    float score = 10.0f - ((diff / max_possible) * 10.0f);
    printf("Diff: %f, Score: %f\n", diff, score);
    return 0;
}
