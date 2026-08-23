#ifndef ASSESSMENT_H
#define ASSESSMENT_H

typedef struct {
    int   studentRef;
    
    /* 1-5 Descriptor Skill Levels */
    int   level_programming;
    int   level_networking;
    int   level_design;
    int   level_analytics;
    int   level_communication;
    int   level_security;

    /* Corresponding float scores (2.0 to 10.0) */
    float programming;
    float networking;
    float design;
    float analytics;
    float communication;
    float security;

    /* Major CSE Courses (1 = completed, 0 = not completed) */
    int   course_ml_ai;
    int   course_web_dev;
    int   course_data_science;
    int   course_cybersecurity;
    int   course_cloud_computing;

    /* Technology Stack Checklist (1 = known, 0 = not known) */
    int   lang_python;
    int   lang_java;
    int   lang_c_cpp;
    int   lang_js;
    int   lang_php;
    int   lang_go;
    int   lang_kotlin;
    int   lang_sql;

    int   tool_git;
    int   tool_docker;
    int   tool_linux;
    int   tool_cloud;
    int   tool_react_vue;
    int   tool_tf_pytorch;
    int   tool_figma;
    int   tool_mongodb;

    /* Project Experience (1 = built, 0 = not built) */
    int   proj_web;
    int   proj_mobile;
    int   proj_db;
    int   proj_ai;
    int   proj_security;

    /* Work / Competition Experience (1 = yes, 0 = no) */
    int   exp_internship;
    int   exp_hackathon;
    int   exp_competitive_prog;
    int   exp_research;

    /* Primary Career Interest (1=Software, 2=Data/AI, 3=Security, 4=Cloud, 5=Design) */
    int   career_interest;
} SkillProfile;

float levelToScore(int level);
const char* levelName(int level);

SkillProfile runAssessment(int studentRef);

#endif