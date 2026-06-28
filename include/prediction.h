#ifndef PREDICTION_H
#define PREDICTION_H

#include "assessment.h"

#define NUM_CAREERS 6

typedef struct {
    char  name[50];
    float weights[6];
    float minRequired[6];
} CareerPath;

typedef struct {
    int   studentRef;
    char  timestamp[30];
    char  topCareer[50];
    float score;
} PredictionRecord;

void  rankCareers(SkillProfile *sp);
float calculateScore(SkillProfile *sp, CareerPath *cp);

#endif