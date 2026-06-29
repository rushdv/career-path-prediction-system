#ifndef ASSESSMENT_H
#define ASSESSMENT_H

typedef struct {
    int   studentRef;
    float programming;
    float networking;
    float design;
    float analytics;
    float communication;
    float security;
} SkillProfile;

SkillProfile runAssessment(int studentRef);

#endif