#ifndef PREDICTION_H
#define PREDICTION_H

#include "assessment.h"

#define NUM_CAREERS 40

typedef struct {
    int studentRef;
    char topCareer[100];
    float score;
    char timestamp[30];
} PredictionRecord;

typedef struct {
    char  name[50];
    int   career_type;  /* 1=Software, 2=Data/AI, 3=Security, 4=Cloud, 5=Design */
    float req_programming;
    float req_networking;
    float req_design;
    float req_analytics;
    float req_communication;
    float req_security;
    
    /* Weights for having relevant courses */
    float weight_ml_ai;
    float weight_web_dev;
    float weight_data_science;
    float weight_cybersecurity;
    float weight_cloud_computing;
} CareerPath;

void getCareers(CareerPath *careers, int *count);
float calculateScore(const SkillProfile *sp, const CareerPath *cp);
void rankCareers(const SkillProfile *sp);

#endif