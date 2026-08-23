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
