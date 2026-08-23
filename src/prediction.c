#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prediction.h"
#include "colors.h"
#include "db_handler.h"

/* ── Provide list of available careers ──────────────────── */
void getCareers(CareerPath *careers, int *count) {
    *count = 0;
    FILE *fp = fopen("data/careers.csv", "r");
    if (!fp) {
        printError("Could not open data/careers.csv! Ensure the file exists.");
        return;
    }

    char line[256];
    int first_line = 1;

    while (fgets(line, sizeof(line), fp)) {
        if (first_line) {
            first_line = 0;
            continue; /* Skip header */
        }
        
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        /* Parse 13 CSV values */
        char *token = strtok(line, ",");
        if (!token) continue;

        strcpy(careers[*count].name, token);
        
        token = strtok(NULL, ","); if(token) careers[*count].career_type = atoi(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_programming = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_networking = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_design = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_analytics = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_communication = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].req_security = atof(token);
        
        token = strtok(NULL, ","); if(token) careers[*count].weight_ml_ai = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].weight_web_dev = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].weight_data_science = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].weight_cybersecurity = atof(token);
        token = strtok(NULL, ","); if(token) careers[*count].weight_cloud_computing = atof(token);

        (*count)++;
        if (*count >= NUM_CAREERS) break; /* Avoid overflowing if NUM_CAREERS is small */
    }
    fclose(fp);
}

/* ── Calculate matching score ───────────────────────────── */
float calculateScore(const SkillProfile *sp, const CareerPath *cp) {
    /* 1. Core Engineering Competencies (Max 6.5 points / 65%) */
    float req_total = cp->req_programming + cp->req_networking + cp->req_design +
                      cp->req_analytics   + cp->req_communication + cp->req_security;
    if (req_total < 1.0f) req_total = 1.0f;

    float fulfilled = 0.0f;
    float excess    = 0.0f;

    #define EVAL_SKILL(user_val, req_val) do { \
        if ((user_val) <= (req_val)) { \
            fulfilled += (user_val); \
        } else { \
            fulfilled += (req_val); \
            excess += ((user_val) - (req_val)); \
        } \
    } while(0)

    EVAL_SKILL(sp->programming,   cp->req_programming);
    EVAL_SKILL(sp->networking,    cp->req_networking);
    EVAL_SKILL(sp->design,        cp->req_design);
    EVAL_SKILL(sp->analytics,     cp->req_analytics);
    EVAL_SKILL(sp->communication, cp->req_communication);
    EVAL_SKILL(sp->security,      cp->req_security);
    #undef EVAL_SKILL

    float base_ratio = fulfilled / req_total;
    float core_score = (base_ratio * 6.0f) + ((excess / req_total) * 0.5f);
    if (core_score > 6.5f) core_score = 6.5f;

    /* 2. Specialized Coursework Bonus (Max 1.5 points / 15%) */
    float course_bonus = 0.0f;
    if (sp->course_ml_ai)           course_bonus += cp->weight_ml_ai;
    if (sp->course_web_dev)         course_bonus += cp->weight_web_dev;
    if (sp->course_data_science)    course_bonus += cp->weight_data_science;
    if (sp->course_cybersecurity)   course_bonus += cp->weight_cybersecurity;
    if (sp->course_cloud_computing) course_bonus += cp->weight_cloud_computing;
    if (course_bonus > 1.5f) course_bonus = 1.5f;

    /* 3. Tech Stack & Practical Projects (Max 1.0 point / 10%) */
    int tech_count = sp->lang_python + sp->lang_java + sp->lang_c_cpp + sp->lang_js +
                     sp->lang_php + sp->lang_go + sp->lang_kotlin + sp->lang_sql +
                     sp->tool_git + sp->tool_docker + sp->tool_linux + sp->tool_cloud +
                     sp->tool_react_vue + sp->tool_tf_pytorch + sp->tool_figma + sp->tool_mongodb;
    float tech_bonus = (tech_count * 0.035f);
    if (tech_bonus > 0.5f) tech_bonus = 0.5f;

    int proj_count = sp->proj_web + sp->proj_mobile + sp->proj_db + sp->proj_ai + sp->proj_security;
    float proj_bonus = (proj_count * 0.10f);
    if (proj_bonus > 0.5f) proj_bonus = 0.5f;

    /* 4. Industry Experience & Extracurriculars (Max 0.5 points / 5%) */
    float exp_bonus = 0.0f;
    if (sp->exp_internship)       exp_bonus += 0.25f;
    if (sp->exp_hackathon)        exp_bonus += 0.10f;
    if (sp->exp_competitive_prog) exp_bonus += 0.10f;
    if (sp->exp_research)         exp_bonus += 0.10f;
    if (exp_bonus > 0.5f) exp_bonus = 0.5f;

    /* 5. Career Goal / Primary Interest Alignment (Max 0.5 points / 5%) */
    float interest_bonus = 0.0f;
    if (sp->career_interest > 0 && sp->career_interest == cp->career_type) {
        interest_bonus = 0.5f;
    }

    /* Final combined score (0.0 to 10.0) */
    float total_score = core_score + course_bonus + tech_bonus + proj_bonus + exp_bonus + interest_bonus;
    if (total_score > 10.0f) total_score = 10.0f;
    if (total_score < 0.0f)  total_score = 0.0f;

    return total_score;
}
